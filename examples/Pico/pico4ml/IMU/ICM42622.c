#include "ICM42622.h"

#include "stdio.h"

bool reserved_addr(uint8_t addr)
{
  return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}
uint8_t icm42622_init(void)
{

  i2c_init(I2C_PORT, 400 * 1000);
  gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(SDA_PIN);
  gpio_pull_up(SCL_PIN);

  uint8_t Ascale = AFS_4G, Gscale = GFS_2000DPS, AODR = AODR_100Hz, GODR = GODR_100Hz;
  uint8_t DeviceID = 0;

  // printf("\nI2C Bus Scan\n");
  // printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
  // for (int addr = 0; addr < (1 << 7); ++addr)
  // {
  //   if (addr % 16 == 0)
  //   {
  //     printf("%02x ", addr);
  //   }

  //   //	   Perform a 1-byte dummy read from the probe address. If a slave
  //   //	   acknowledges this address, the function returns the number of bytes
  //   //	   transferred. If the address byte is ignored, the function returns
  //   //	   -1.
  //   //
  //   //	   Skip over any reserved addresses.
  //   int ret;
  //   uint8_t rxdata;
  //   if (reserved_addr(addr))
  //     ret = PICO_ERROR_GENERIC;
  //   else
  //     ret = i2c_read_blocking(I2C_PORT, addr, &rxdata, 1, false);

  //   printf(ret < 0 ? "." : "@");
  //   printf(addr % 16 == 15 ? "\n" : "  ");
  // }
  // printf("Done.\n");

  DeviceID = icm42622_check_ID();
  if (DeviceID != ICM42622_DEVICE_ID)
  {
    return 0;
  }

  uint8_t temp = i2c_read_one_byte(ICM42622_DEVICE_CONFIG);
  i2c_write_one_byte(ICM42622_DEVICE_CONFIG, temp | 0x01);
  sleep_ms(1000);

  temp = i2c_read_one_byte(ICM42622_PWR_MGMT0); // make sure not to disturb reserved bit values

  i2c_write_one_byte(ICM42622_PWR_MGMT0, 0x0F); // enable gyro and accel in low noise mode

  sleep_ms(100);

  i2c_write_one_byte(ICM42622_GYRO_CONFIG0, GODR | Gscale << 5); // gyro full scale and data rate

  i2c_write_one_byte(ICM42622_ACCEL_CONFIG0, AODR | Ascale << 5); // set accel full scale and data rate

  i2c_write_one_byte(ICM42622_GYRO_CONFIG1, 0x0a); // set temperature sensor low pass filter to 5Hz, use first order gyro filter

  i2c_write_one_byte(ICM42622_ACCEL_CONFIG1, 0x14);

  i2c_write_one_byte(ICM42622_INT_CONFIG, 0x03); // set both interrupts active high, push-pull, pulsed

  i2c_write_one_byte(ICM42622_FIFO_CONFIG, 0x40);

  i2c_write_one_byte(ICM42622_FIFO_CONFIG1, 0x61); // Enable the accel  to the FIFO

  i2c_write_one_byte(ICM42622_FIFO_CONFIG2, 0x00);

  i2c_write_one_byte(ICM42622_FIFO_CONFIG3, 0xff);
  return 1;
}

bool icm42622_read_gyro(float *ps16X, float *ps16Y, float *ps16Z)
{
  uint8_t u8Buf[6] = {0};
  int16_t s16Buf[3] = {0};

  u8Buf[0] = i2c_read_one_byte(ICM42622_GYRO_DATA_X0);
  u8Buf[1] = i2c_read_one_byte(ICM42622_GYRO_DATA_X1);
  s16Buf[0] = (u8Buf[1] << 8) | u8Buf[0];
  u8Buf[0] = i2c_read_one_byte(ICM42622_GYRO_DATA_Y0);
  u8Buf[1] = i2c_read_one_byte(ICM42622_GYRO_DATA_Y1);
  s16Buf[1] = (u8Buf[1] << 8) | u8Buf[0];
  u8Buf[0] = i2c_read_one_byte(ICM42622_GYRO_DATA_Z0);
  u8Buf[1] = i2c_read_one_byte(ICM42622_GYRO_DATA_Z1);
  s16Buf[2] = (u8Buf[1] << 8) | u8Buf[0];

  *ps16X = s16Buf[0] * 2000.0 / 32768.0;
  *ps16Y = s16Buf[1] * 2000.0 / 32768.0;
  *ps16Z = s16Buf[2] * 2000.0 / 32768.0;

  if (*ps16X == 0 && *ps16Y == 0 && *ps16Z == 0)
  {
    return false;
  }

  return true;
}

bool icm42622_read_accel(float *ps16X, float *ps16Y, float *ps16Z)
{
  uint8_t u8Buf[2];
  int16_t s16Buf[3] = {0};

  u8Buf[0] = i2c_read_one_byte(ICM42622_ACCEL_DATA_X0);
  u8Buf[1] = i2c_read_one_byte(ICM42622_ACCEL_DATA_X1);
  s16Buf[0] = (u8Buf[1] << 8) | u8Buf[0];

  u8Buf[0] = i2c_read_one_byte(ICM42622_ACCEL_DATA_Y0);
  u8Buf[1] = i2c_read_one_byte(ICM42622_ACCEL_DATA_Y1);
  s16Buf[1] = (u8Buf[1] << 8) | u8Buf[0];

  u8Buf[0] = i2c_read_one_byte(ICM42622_ACCEL_DATA_Z0);
  u8Buf[1] = i2c_read_one_byte(ICM42622_ACCEL_DATA_Z1);
  s16Buf[2] = (u8Buf[1] << 8) | u8Buf[0];

  *ps16X = s16Buf[0] * 4.0 / 32768.0;
  *ps16Y = s16Buf[1] * 4.0 / 32768.0;
  *ps16Z = s16Buf[2] * 4.0 / 32768.0;
  if (*ps16X == 0 && *ps16Y == 0 && *ps16Z == 0)
  {
    return false;
  }

  return true;
}

uint8_t icm42622_check_ID(void)
{
  return i2c_read_one_byte(ICM42622_WHO_AM_I);
}

uint8_t icm42622_data_ready(void)
{
  return i2c_read_one_byte(ICM42622_INT_STATUS) & 0x08;
}

void i2c_write_one_byte(uint8_t reg, uint8_t value)
{
  uint8_t buf[] = {reg, value};
  i2c_write_blocking(I2C_PORT, ICM42622_I2C_ADDRESS, buf, 2, false);
}

uint8_t i2c_read_one_byte(uint8_t reg)
{
  uint8_t buf;
  i2c_write_blocking(I2C_PORT, ICM42622_I2C_ADDRESS, &reg, 1, true);
  i2c_read_blocking(I2C_PORT, ICM42622_I2C_ADDRESS, &buf, 1, false);
  return buf;
}

void icm42622_read_fifo(uint8_t *buffer, uint16_t lenght)
{
  i2c_read_block(ICM42622_FIFO_DATA, buffer, lenght);
}

void i2c_read_block(uint8_t add, uint8_t *buffer, uint16_t lenght)
{
  i2c_write_blocking(I2C_PORT, ICM42622_I2C_ADDRESS, &add, 1, false);
  i2c_read_blocking(I2C_PORT, ICM42622_I2C_ADDRESS, buffer, lenght, false);
}