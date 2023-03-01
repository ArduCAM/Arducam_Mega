#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct Point3d {
    int x;
    int y;
    int z;
};
struct Point2d {
    int x;
    int y;
};

struct Line3d {
    struct Point3d p0;
    struct Point3d p1;
};
struct Line2d {
    struct Point2d p0;
    struct Point2d p1;
};

void SetVars(void);

void ProcessLine(struct Line2d *ret, struct Line3d vec);

void setup();
void loop();
#ifdef __cplusplus
}
#endif
