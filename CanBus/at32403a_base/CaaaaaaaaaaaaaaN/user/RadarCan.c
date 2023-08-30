/*
 * RadarCan.c
 *
 *  Created on: 2023 Apr 20
 *      Author: on4ip
 */
#include "RadarCan.h"
#include "canInit.h"
can_header_t header;
can_cluster_t clusters[MAX_CLUSTERS];

#define NODE_ID 0x100
typedef struct
{
	uint16_t ID;
	uint8_t data[8];
} canPacket_st;
#define CAN_PACKET_NUM (((MAX_CLUSTERS*SIZEOF_CLUSTER)+SIZEOF_HEADER)>>3)
canPacket_st canPacketArray[CAN_PACKET_NUM];
void dataPackToCAN(const can_header_t *pHeader, const can_cluster_t *pCluster)
{

	/*Set node id and header start message*/
	canPacketArray[0].ID = NODE_ID;
	memcpy(&canPacketArray[0].data[0], (uint8_t*) pHeader,
			sizeof(canPacketArray[0].data));
	canPacketArray[1].ID = NODE_ID + 1;
	memcpy(&canPacketArray[1].data[0], (uint8_t*) pHeader + 8,
			sizeof(canPacketArray[0].data));

	/* construct cluster data field*/
	for (uint16_t i = 0; i < header.length; i++)
	{
		uint16_t canPackNum = 2 + 2*i;
		const can_cluster_t *plocalCluster = &pCluster[i];

		canPacketArray[canPackNum].ID = NODE_ID + canPackNum;
		memcpy(&canPacketArray[canPackNum].data[0], (uint8_t*) plocalCluster,
				sizeof(canPacketArray[0].data));
		canPacketArray[canPackNum + 1].ID = NODE_ID + (canPackNum + 1);
		memcpy(&canPacketArray[canPackNum + 1].data[0], (uint8_t*) plocalCluster + 8,
				sizeof(canPacketArray[0].data));
	}

	uint8_t res = 0;
	for (uint16_t i = 0; i < CAN_PACKET_NUM; i++)
	{
		do
		{
			/**
			 * @details here CAN driver send function that send amount out data in blocking mode
			 */
			res = sendToCan(canPacketArray[i].ID, 8,
					&canPacketArray[i].data[0]);
		} while (!res);
	}

}

static uint8_t initDone = 1;

/**
 * @brief Function init clusters data to default
 */
void initClusters()
{
	for (uint16_t i = 0; i < MAX_CLUSTERS; i++)
	{
		clusters[i].id = 0xA0+i;
		clusters[i].x = (int16_t) (i + 1);
		clusters[i].y = (int16_t) (i + 2);
		clusters[i].z = (int16_t) (i + 3);
	}
	initDone = 0;
}

/**
 * @brief Main process function that pacj data to can messages and sent it
 */
void commProcess(void)
{
	if (initDone)
	{
		initClusters();
	}

	header.frame_id = 0x55;
	header.sec = 2;
	header.usec = 123;
	header.length = MAX_CLUSTERS;
	dataPackToCAN(&header, &clusters[0]);
}
