/*
 *  Query_Persist.c
 *
 *  Created on: 2016年6月22日
 *  Author: 04153010_姜文昊
 */
#include "Query_Persist.h"
#include "../Service/play.h"
#include "../Common/list.h"
#include "../Service/studio.h"
#include "../Service/ticket.h"
#include "../Service/seat.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <io.h>
#include <string.h>

static const char PLAY_DATA_FILE[] = "Play.dat";
static const char SCHEDULE_DATA_FILE[] = "Schedule.dat";
static const char STUDIO_DATA_FILE[] = "Studio.dat";
static const char TICKET_DATA_FILE[]="Ticket.dat";
static const char SEAT_DATA_FILE[] = "Seat.dat";
//Query play by name
//0: error, the play is not exits
//1: the play exits and storage the information in the struct

/*根据剧目id查找对应的演出计划*/
int Query_PlayName(char *playName, play_t *_play)
{
	assert(NULL!=_play);
	FILE *fp = fopen(PLAY_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}
	play_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(play_t), 1, fp)) {
			if (strcmp(playName , data.name)==0) {
				*_play = data;
				found = 1;
				break;
			}
		}
	}
	fclose(fp);
	return found;
}
//query schedule by play id
//return 0: error the schedule is note exits
//return 1:the schedule exits and storage the information in struct

/*根据剧目id查找对应的剧场信息*/
int Query_Schedule_By_PlayId(int playId, schedule_t *sched)
{
	assert(NULL != sched);
	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) {
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}
	schedule_t data;
	memset(&data, 0, sizeof(schedule_t));
	while(!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
			if (playId == data.play_id) {
				*sched = data;
				fclose(fp);
				return 1;
			}
		}
	}
	return 0;
}

/*根据剧场id查找对应的剧场信息*/
void Query_Studio_By_Schedule(int id, studio_t *studio)
{
	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) {
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
	}

	schedule_t data;


	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp)) {
			if (id == data.id) {
				//*sched = data;

				FILE *fp1 = fopen(STUDIO_DATA_FILE, "rb");
				if (NULL == fp1) {

				}

				studio_t sdata;


				while (!feof(fp1)) {
					if (fread(&sdata, sizeof(studio_t), 1, fp1)) {
						if (sdata.id == data.studio_id) {
							*studio = sdata;

							break;
						};

					}
				}
				fclose(fp1);
				break;
			};

		}
	}
	fclose(fp);

}

/*根据演出计划id查询已经售出票的个数*/
int Sold_Ticket_Count(int id)
{
	int count = 0;
	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp) {
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	ticket_t data;
	while (!feof(fp)) {
		memset(&data, 0, sizeof(ticket_t));
		if (fread(&data, sizeof(ticket_t), 1, fp)) {
			if ((id == data.schedule_id) && (data.status == 1)) {
				//if (id == data.schedule_id) {
				count++;
			}
		}
	}
	return count;
}

/*根据剧场id查询座位的个数*/
int Seat_Number_Count(int studio_id)
{
	seat_t data;
	int recCount = 0;

	FILE *fp = fopen(SEAT_DATA_FILE, "rb");
	if (NULL == fp) {
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	while (!feof(fp)) {
		if (fread(&data, sizeof(seat_t), 1, fp)) {
				if(studio_id==data.roomID && data.status==SEAT_GOOD) {
			recCount++;
				}
		}
	fclose(fp);
	return recCount;
	}
}
