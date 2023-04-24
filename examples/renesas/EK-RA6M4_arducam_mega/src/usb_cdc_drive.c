#include "common_init.h"
FSP_CPP_HEADER
fsp_err_t usb_cdc_init(void);
bool usb_cdc_stask(uint8_t **cmdline);
fsp_err_t print_to_console(char *p_data);
fsp_err_t usb_cdc_write(uint8_t *p_data, uint32_t len);
FSP_CPP_FOOTER

/* Global variables */
extern uint8_t g_apl_device[];
extern uint8_t g_apl_configuration[];
extern uint8_t g_apl_hs_configuration[];
extern uint8_t g_apl_qualifier_descriptor[];
extern uint8_t *g_apl_string_table[];

const usb_descriptor_t g_usb_descriptor =
    {
        g_apl_device,               /* Pointer to the device descriptor */
        g_apl_configuration,        /* Pointer to the configuration descriptor for Full-speed */
        g_apl_hs_configuration,     /* Pointer to the configuration descriptor for Hi-speed */
        g_apl_qualifier_descriptor, /* Pointer to the qualifier descriptor */
        g_apl_string_table,         /* Pointer to the string descriptor table */
        NUM_STRING_DESCRIPTOR};

usb_status_t usb_event;

static bool b_usb_attach = false;
static uint8_t g_buf[READ_BUF_SIZE] = {0};
// static usb_event_info_t event_info = {0};
static usb_pcdc_linecoding_t g_line_coding;

static fsp_err_t check_for_write_complete(void)
{
    usb_status_t usb_write_event = USB_STATUS_NONE;
    int32_t timeout_count = UINT16_MAX;
    fsp_err_t err = FSP_SUCCESS;
    usb_event_info_t event_info = {0};

    do
    {
        err = R_USB_EventGet(&event_info, &usb_write_event);
        if (FSP_SUCCESS != err)
            return err;

        --timeout_count;

        if (0 > timeout_count)
        {
            timeout_count = 0;
            err = (fsp_err_t)USB_STATUS_NONE;
            break;
        }
    } while (USB_STATUS_WRITE_COMPLETE != usb_write_event);

    return err;
}

fsp_err_t print_to_console(char *p_data)
{
    fsp_err_t err = FSP_SUCCESS;
    uint32_t len = ((uint32_t)strlen(p_data));

    if (!b_usb_attach) return err;
    err = R_USB_Write(&g_basic0_ctrl, (uint8_t *)p_data, len, USB_CLASS_PCDC);
    /* Handle error */
    if (FSP_SUCCESS != err)
    {
        return err;
    }

    err = check_for_write_complete();
    if (FSP_SUCCESS != err)
    {
        /* Did not get the event hence returning error */
        return FSP_ERR_USB_FAILED;
    }
    return err;
}

fsp_err_t usb_cdc_write(uint8_t *p_data, uint32_t len)
{
    fsp_err_t err = FSP_SUCCESS;

    if (!b_usb_attach) return err;
    err = R_USB_Write(&g_basic0_ctrl, p_data, len, USB_CLASS_PCDC);
    /* Handle error */
    if (FSP_SUCCESS != err)
        return err;

    err = check_for_write_complete();
    if (FSP_SUCCESS != err)
    {
        /* Did not get the event hence returning error */
        return FSP_ERR_USB_FAILED;
    }
    return err;
}



fsp_err_t usb_cdc_init(void)
{
    return R_USB_Open(&g_basic0_ctrl, &g_basic0_cfg);
}

bool usb_cdc_stask(uint8_t **cmdline)
{
    /* Loop back between PC(TerminalSoft) and USB MCU */
    usb_event_info_t event_info = {0};
    
    fsp_err_t err = R_USB_EventGet(&event_info, &usb_event);
    /* Handle error */
    if (FSP_SUCCESS != err)
    {
        /* Turn ON RED LED to indicate fatal error */
        // TURN_RED_ON
        APP_ERR_TRAP(err);
    }

    /* USB event received by R_USB_EventGet */
    switch (usb_event)
    {
    case USB_STATUS_CONFIGURED:
//    {
//        err = R_USB_Read(&g_basic0_ctrl, g_buf, READ_BUF_SIZE, USB_CLASS_PCDC);
//        /* Handle error */
//        if (FSP_SUCCESS != err)
//        {
//            /* Turn ON RED LED to indicate fatal error */
//            // TURN_RED_ON
//            APP_ERR_TRAP(err);
//        }
//        break;
//    }
    case USB_STATUS_READ_COMPLETE:
    {
        if (b_usb_attach)
        {
            err = R_USB_Read(&g_basic0_ctrl, g_buf, READ_BUF_SIZE, USB_CLASS_PCDC);
        }
        *cmdline = g_buf;
        /* Handle error */
        if (FSP_SUCCESS != err)
        {
            /* Turn ON RED LED to indicate fatal error */
            // TURN_RED_ON
            APP_ERR_TRAP(err);
        }
//        if(f_buf == 0x55)
//        {
//            g_buf_idx = 0;
//        } else if(f_buf == 0xaa && g_buf_idx < ){
//            cmdline = g_buf;
            return true;
//        }
//        if(g_buf_idx < READ_BUF_SIZE)
//        {
//            g_buf[g_buf_idx] = f_buf;
//        }
//        g_buf_idx++;
        break;
    }
    case USB_STATUS_REQUEST: /* Receive Class Request */
    {
        /* Check for the specific CDC class request IDs */
        if (USB_PCDC_SET_LINE_CODING == (event_info.setup.request_type & USB_BREQUEST))
        {
            err = R_USB_PeriControlDataGet(&g_basic0_ctrl, (uint8_t *)&g_line_coding, LINE_CODING_LENGTH);
            /* Handle error */
            if (FSP_SUCCESS != err)
            {
                /* Turn ON RED LED to indicate fatal error */
                // TURN_RED_ON
                APP_ERR_TRAP(err);
            }
        }
        else if (USB_PCDC_GET_LINE_CODING == (event_info.setup.request_type & USB_BREQUEST))
        {
            err = R_USB_PeriControlDataSet(&g_basic0_ctrl, (uint8_t *)&g_line_coding, LINE_CODING_LENGTH);
            /* Handle error */
            if (FSP_SUCCESS != err)
            {
                /* Turn ON RED LED to indicate fatal error */
                // TURN_RED_ON
                APP_ERR_TRAP(err);
            }
        }
        else if (USB_PCDC_SET_CONTROL_LINE_STATE == (event_info.setup.request_type & USB_BREQUEST))
        {
            err = R_USB_PeriControlStatusSet(&g_basic0_ctrl, USB_SETUP_STATUS_ACK);
            /* Handle error */
            if (FSP_SUCCESS != err)
            // if (FSP_SUCCESS != g_err)
            {
                /* Turn ON RED LED to indicate fatal error */
                // TURN_RED_ON
                APP_ERR_TRAP(err);
            }
        }
        else
        {
            /* none */
        }
        break;
    }
    case USB_STATUS_DETACH:
    case USB_STATUS_SUSPEND:
    {
        b_usb_attach = false;
//        memset(g_buf, 0, sizeof(g_buf));
        break;
    }
    case USB_STATUS_RESUME:
    {
        b_usb_attach = true;
        break;
    }
    default:
        break;
    }
    return false;
} /* End of function usb_main() */
