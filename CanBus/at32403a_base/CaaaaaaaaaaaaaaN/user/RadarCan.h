/*
 * RadarCan.h
 *
 *  Created on: 2023 Apr 20
 *      Author: on4ip
 */

#ifndef USER_RADARCAN_H_
#define USER_RADARCAN_H_
#include "stdint.h"
#include "stddef.h"
/* memcpy using*/
#include "string.h"
typedef struct
{
	uint16_t frame_id;
	uint16_t reserved0;
	uint32_t sec;
	uint32_t usec;
	uint16_t reserved1;
	uint16_t length;
} can_header_t;

typedef struct
{
	uint8_t id;
	uint8_t reserved0;
	int16_t x;
	int16_t y;
	int16_t z;
	int16_t v;
	uint8_t length;
	uint8_t width;
	uint8_t height;
	int8_t magitude;
	uint16_t reserved1;
} can_cluster_t;
#define MAX_CLUSTERS			(4)
#define SIZEOF_HEADER (sizeof(can_header_t))
#define SIZEOF_CLUSTER (sizeof(can_cluster_t))

/*
 * @brief communication function
 */
void commProcess(void);

void dataPackToCAN(const can_header_t *pHeare,const can_cluster_t *pCluster);

#endif /* USER_RADARCAN_H_ */
