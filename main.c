/*
 * Date : 2016.05.24
 * Subject : Programming_Project
 * Author : Suchang Ko
 * Project : Building Simulation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
FILE *fp;


int tts_flag = 0;

typedef struct _space {
    char *user; // 주민
    struct _space *space_next; // 다음 노드
} space;

typedef space *space_ptr; // Space Pointer

typedef struct _room {
    int room_id; // 방의 호수
    int space_count; // 방의 칸수
    struct _room *room_next; // 다음 노드
    space_ptr space_head; // 칸 리스트
} room;

typedef room *room_ptr; // Room Pointer

typedef struct _floor {
    int floor_id; // 층
    int room_count; // 총 방 수
    struct _floor *floor_next; // 다음 노드
    room_ptr room_head;
} floor;

typedef floor *floor_ptr; // Floor Pointer

typedef struct _building {
    int floor_count; // 총 층 수
    floor_ptr floor_head;
} building;

void log_time() {
    time_t timer;
    struct tm *t;

    timer = time(NULL);
    t = localtime(&timer);
    fprintf(fp, "[%d-%02d-%02d %02d:%02d:%02d] ", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min,
            t->tm_sec);
}

void init(building *building_ptr) {
    building_ptr->floor_count = 0;
    building_ptr->floor_head = NULL;
}

int input_cmd() {

    while (1) {
        int cmd = 0;
        printf("\n기능을 선택하세요 [메인]\n");
        printf("[1] 건물 확장 (층,방) \n");
        printf("[2] View 변환\n");
        printf("[3] 방별 칸 변환\n");
        printf("[4] 칸 별 사람 이름 변환\n");
        printf("[5] 사람 검색\n");
        printf("[6] TTS ON/OFF\n");
        printf("[7] 종료\n");
        if (tts_flag) {
            system("pico2wave -w test.wav \" Choose Option. Main, Building Expansion.\" ");
            system("aplay -q test.wav");
            system("pico2wave -w test.wav \"1, building expansion. \" ");
            system("aplay -q test.wav");
            system("pico2wave -w test.wav \"2, translate view.\" ");
            system("aplay -q test.wav");
            system("pico2wave -w test.wav \"3, change section for each room.\"");
            system("aplay -q test.wav");
            system("pico2wave -w test.wav \"4, change person for each section.\"");
            system("aplay -q test.wav");
            system("pico2wave -w test.wav \"5, search person.\"");
            system("aplay -q test.wav");
            system("pico2wave -w test.wav \"6, text to speach on or off.\"");
            system("aplay -q test.wav");
            system("pico2wave -w test.wav \"7, exit program.\"");
            system("aplay -q test.wav");
        }
        scanf("%d", &cmd);
        if (cmd >= 1 && cmd <= 7) {
            return cmd;
        } else {
            printf("다시 입력해 주세요.\n");
            if (tts_flag){
                system("pico2wave -w test.wav \"Try again.\"");
                system("aplay -q test.wav");
            }
        }
    }
}

int input_extend_cmd() {
    while (1) {
        int cmd = 0;
        printf("\n기능을 선택하세요 [메인]-[건물확장]\n");
        printf("[1] 층 확장\n");
        printf("[2] 방 확장\n");
        printf("[3] 뒤로가기\n");
        if (tts_flag) {
            system("pico2wave -w test.wav \" Choose Option. Main, Building Expansion.\" ");
            system("aplay -q test.wav");
            system("pico2wave -w test.wav \"1, add floor to this building.\"");
            system("aplay -q test.wav");
            system("pico2wave -w test.wav \"2, add room to specific floor.\"");
            system("aplay -q test.wav");
            system("pico2wave -w test.wav \"3, Go back to previous option.\"");
            system("aplay -q test.wav");
        }
        scanf("%d", &cmd);
        if (cmd >= 1 && cmd <= 3) {
            return cmd;
        } else {
            printf("다시 입력해 주세요.\n");
            if (tts_flag){
                system("pico2wave -w test.wav \"Try again.\"");
                system("aplay -q test.wav");
            }
        }
    }
}

int input_view_cmd() {
    while (1) {
        int cmd = 0;
        printf("\n기능을 선택하세요 [메인]-[View 변환]\n");
        printf("[1] 건물 전체 View\n");
        printf("[2] 방 내부 View\n");
        printf("[3] 뒤로가기\n");
        if (tts_flag) {
            system("pico2wave -w test.wav \"Choose Option. Main, Translate View.\"");
            system("aplay -q test.wav");
            system("pico2wave -w test.wav \"1, entire view of building.\"");
            system("aplay -q test.wav");
            system("pico2wave -w test.wav \"2, specific view of room.\"");
            system("aplay -q test.wav");
            system("pico2wave -w test.wav \"3, Go back to previous option.\"");
            system("aplay -q test.wav");
        }
        scanf("%d", &cmd);
        if (cmd >= 1 && cmd <= 3) {
            return cmd;
        } else {
            printf("다시 입력해 주세요.\n");
            if (tts_flag){
                system("pico2wave -w test.wav \"Try again.\"");
                system("aplay -q test.wav");
            }
        }
    }
}

void view_building(building *building_list, int msg) {
    printf("\n===============================\n");
    int tmp[100][2] = {0,};
    int i;
    int floor_index, room_index, space_index;
    int total_room_count = 0, total_space_count = 0, total_user_count = 0;
    floor_ptr tmp_floor_ptr = building_list->floor_head;
    for (floor_index = 0; floor_index < building_list->floor_count; floor_index++) {
        total_room_count += tmp_floor_ptr->room_count;
        room_ptr tmp_room_ptr = tmp_floor_ptr->room_head;
        for (room_index = 0; room_index < tmp_floor_ptr->room_count; room_index++) {
            space_ptr tmp_space_ptr = tmp_room_ptr->space_head;
            total_space_count += tmp_room_ptr->space_count;
            tmp[tmp_room_ptr->room_id][0] = tmp_room_ptr->room_id;
            tmp[tmp_room_ptr->room_id][1] = tmp_room_ptr->space_count;
            for (space_index = 0; space_index < tmp_room_ptr->space_count; space_index++) {
                //printf(" %d칸 : %s ", (space_index+1),tmp_space_ptr->user);
                if (strlen(tmp_space_ptr->user) > 0) {
                    total_user_count++;
                }
                tmp_space_ptr = tmp_space_ptr->space_next;
            }
            tmp_room_ptr = tmp_room_ptr->room_next;
        }
        for (i = 1; i <= tmp_floor_ptr->room_count; i++) {
            printf("╠═══════════╣");
        }
        printf("\n");
        for (i = 1; i <= tmp_floor_ptr->room_count; i++) {
            printf("║%d%02d호 : %d명║", tmp_floor_ptr->floor_id, tmp[i][0], tmp[i][1]);
        }
        printf("\n");
        for (i = 1; i <= tmp_floor_ptr->room_count; i++) {
            printf("╠═══════════╣");
        }
        printf("\n");
        tmp_floor_ptr = tmp_floor_ptr->floor_next;
    }

    printf("\n현재 거주자 수 : %d명\n", total_user_count);
    printf("현재  공간  수 : %d명\n", total_space_count);
    printf("현재   방   수 : %d명\n", total_room_count);
    printf("\n===============================\n");
    if (tts_flag) {
        char buff[256];
        sprintf(buff, "pico2wave -w test.wav \"There are %d residents.\"", total_user_count);
        system(buff);
        system("aplay -q test.wav");
        memset(buff, '\0', 256);

        sprintf(buff, "pico2wave -w test.wav \"There are %d sections.\"", total_space_count);
        system(buff);
        system("aplay -q test.wav");
        memset(buff, '\0', 256);

        sprintf(buff, "pico2wave -w test.wav \"There are %d rooms.\"", total_room_count);
        system(buff);
        system("aplay -q test.wav");
        memset(buff, '\0', 256);
    }
}

void view_room(building *building_list, int target_floor, int target_room) {
    int floor_index, room_index, space_index;
    floor_ptr tmp_floor_ptr = building_list->floor_head;
    for (floor_index = target_floor; floor_index < building_list->floor_count; floor_index++) {
        tmp_floor_ptr = tmp_floor_ptr->floor_next;
    }
    room_ptr tmp_room_ptr = tmp_floor_ptr->room_head;
    for (room_index = target_room; room_index < tmp_floor_ptr->room_count; room_index++) {
        /*
        tmp_space_ptr = tmp_room_ptr->space_head;
        for (space_index = 0; space_index < tmp_room_ptr->space_count; space_index++) {
            //printf(" %d칸 : %s ", (space_index+1),tmp_space_ptr->user);
            tmp_space_ptr = tmp_space_ptr->space_next;
        }
        */
        tmp_room_ptr = tmp_room_ptr->room_next;
    }
    space_ptr tmp_space_ptr = tmp_room_ptr->space_head;
    printf("===============================\n");
    for (space_index = 0; space_index < tmp_room_ptr->space_count; space_index++) {
        printf(" %d칸 : %s \n", (space_index + 1), tmp_space_ptr->user);
        tmp_space_ptr = tmp_space_ptr->space_next;
    }
    printf("===============================\n");
}

void add_room(building *building_list, int msg) {
    char buff[256];
    if (building_list->floor_count < 1) {
        printf("생성된 층이 없습니다.\n");
        if (tts_flag) {
            system("pico2wave -w test.wav \"There is no added floor.\"");
            system("aplay -q test.wav");
        }
    } else {
        int target_floor;
        if (msg == TRUE) {
            while (1) {
                printf("[현재 층 수] : %d 층\n", building_list->floor_count);
                printf("몇 층의 방을 늘리겠습니까?\n");
                if (tts_flag) {
                    sprintf(buff,
                            "pico2wave -w test.wav \"Current floor number is %d. Which floor you wanted to add room?\"",
                            building_list->floor_count);
                    system(buff);
                    system("aplay -q test.wav");
                    memset(buff, '\0', 256);
                }
                scanf("%d", &target_floor);
                if (target_floor >= 0 && target_floor <= building_list->floor_count) {
                    break;
                } else {
                    printf("현재 층 수를 초과하였습니다. 다시 입력해 주세요.\n");
                    if (tts_flag) {
                        system("pico2wave -w test.wav \"Floor number exceeded. Try again.\"");
                        system("aplay -q test.wav");
                    }
                }
            }
        } else {
            target_floor = building_list->floor_count;
        }


        int index = 0;
        floor_ptr tmp_floor_ptr = building_list->floor_head;
        for (index = 0; index < building_list->floor_count; index++) {
            if (target_floor == tmp_floor_ptr->floor_id) {
                room_ptr new_room_ptr = (room *) malloc(sizeof(room));
                new_room_ptr->room_id = ++tmp_floor_ptr->room_count;
                new_room_ptr->space_count = 0;
                new_room_ptr->room_next = tmp_floor_ptr->room_head;
                tmp_floor_ptr->room_head = new_room_ptr;
                // 방의 기본 성질
                // 각 방에는 1인 이상 살고 있어야 한다.
                char *user_name;
                if (msg == FALSE) {
                    char *tmp_name = "Suchang";
                    user_name = (char *) malloc(strlen(tmp_name) + 1);
                    strcpy(user_name, tmp_name);
                } else {
                    printf("입주자 이름을 입력하세요.\n");
                    if (tts_flag) {
                        system("pico2wave -w test.wav \"Input resident's name.\"");
                        system("aplay -q test.wav");
                    }
                    scanf("%s", &user_name);
                }

                space_ptr new_space_ptr = (space *) malloc(sizeof(space));
                new_space_ptr->user = (char *) malloc(strlen(&user_name + 1));
                if (msg)
                    strcpy(new_space_ptr->user, &user_name);
                else
                    strcpy(new_space_ptr->user, user_name);
                new_space_ptr->space_next = new_room_ptr->space_head;
                new_room_ptr->space_head = new_space_ptr;
                new_room_ptr->space_count++;
                if (msg == TRUE)
                    printf("입주자 : %s\n", new_space_ptr->user);
                break;
            }
            tmp_floor_ptr = tmp_floor_ptr->floor_next;
        }
        if (msg == TRUE)
            printf("%d 층의 방 수 : %d개\n", target_floor, tmp_floor_ptr->room_count);
        log_time();
        fprintf(fp, "[add_room] %dF %dRoom = %d\n", target_floor, tmp_floor_ptr->room_count);
    }
}

void add_floor(building *building_list, int msg) {
    char buff[256];
    floor_ptr new_floor_ptr = (floor *) malloc(sizeof(floor));
    new_floor_ptr->floor_id = ++building_list->floor_count;
    new_floor_ptr->room_count = 0;
    new_floor_ptr->floor_next = building_list->floor_head;
    building_list->floor_head = new_floor_ptr;
    if (msg) {
        printf("추가 완료\n");
        printf("현재 층 수 : %d층\n", building_list->floor_count);
        if (tts_flag) {
            system("pico2wave -w test.wav \"Adding floor complete.\"");
            system("aplay -q test.wav");

            sprintf(buff, "pico2wave -w test.wav \"Currently, this building is %d level.\"",
                    building_list->floor_count);
            system(buff);
            system("aplay -q test.wav");
            memset(buff, '\0', 256);

        }
    }
    log_time();
    fprintf(fp, "[add_floor] now_floor = %d\n", building_list->floor_count);
}

void change_space(building *building_list) {
    char buff[256];
    int target_floor, target_room, target_count;
    printf("몇 층 입니까? 1 ~ %d 층\n", building_list->floor_count);
    if (tts_flag) {
        sprintf(buff, "pico2wave -w test.wav \"On a scale of 1 to %d, which floor?\"", building_list->floor_count);
        system(buff);
        system("aplay -q test.wav");
        memset(buff, '\0', 256);
    }
    scanf("%d", &target_floor);
    floor_ptr tmp_floor_ptr = building_list->floor_head;
    int index = 0;
    for (index = 0; index < building_list->floor_count; index++) {
        if (target_floor == tmp_floor_ptr->floor_id) {
            break;
        }
        tmp_floor_ptr = tmp_floor_ptr->floor_next;
    }
    printf("몇 호 입니까? 1 ~ %d 호\n", tmp_floor_ptr->room_count);
    if (tts_flag) {
        sprintf(buff, "pico2wave -w test.wav \"On a scale of 1 to %d, What's the room number?\"",
                building_list->floor_count);
        system(buff);
        system("aplay -q test.wav");
        memset(buff, '\0', 256);


    }
    scanf("%d", &target_room);
    room_ptr tmp_room_ptr = tmp_floor_ptr->room_head;
    for (index = 0; index < tmp_floor_ptr->room_count; index++) {
        if (target_room == tmp_room_ptr->room_id) {
            break;
        }
        tmp_room_ptr = tmp_room_ptr->room_next;
    }

    while (1) {
        printf("몇 칸 입니까? 1 ~ 4 칸 [현재] : %d칸 \n", tmp_room_ptr->space_count);
        if (tts_flag) {
            system("pico2wave -w test.wav \"On a scale of 1 to 4, how many serctions in this room?\"");
            system("aplay -q test.wav");
        }
        scanf("%d", &target_count);
        if (target_count >= 1 && target_count <= 4) {
            break;
        } else {
            printf("다시 입력 해주세요.\n");
            if (tts_flag) {
                system("pico2wave -w test.wav \"Try again.\"");
                system("aplay -q test.wav");
            }
        }
    }
    if (tmp_room_ptr->space_count == target_count) {
        printf("그대로 유지합니다.\n");
        if (tts_flag) {
            system("pico2wave -w test.wav \"maintain the condition.\"");
            system("aplay -q test.wav");
        }
    } else if (tmp_room_ptr->space_count > target_count) {
        printf("[제거] %d 만큼 반복\n", (tmp_room_ptr->space_count - target_count));
        int count; //remove
        int add_count = tmp_room_ptr->space_count - target_count;
        for (count = 0; count < add_count; count++) {
            space_ptr new_space_ptr = tmp_room_ptr->space_head;
            free(new_space_ptr->user);
            tmp_room_ptr->space_head = new_space_ptr->space_next;
            tmp_room_ptr->space_count--;

        }
    } else if (tmp_room_ptr->space_count < target_count) {
        printf("[추가] %d 만큼 반복\n", (target_count - tmp_room_ptr->space_count));
        int count; //add
        int add_count = target_count - tmp_room_ptr->space_count;
        for (count = 0; count < add_count; count++) {
            char *user_name;
            /*
            printf("입주자 이름을 입력하세요.\n");
            scanf("%s", &user_name);
            */
            char *tmp_name = "\0";
            user_name = (char *) malloc(strlen(tmp_name) + 1);
            strcpy(user_name, tmp_name);

            space_ptr new_space_ptr = (space *) malloc(sizeof(space));
            new_space_ptr->user = (char *) malloc(strlen(&user_name + 1));

            //strcpy(new_space_ptr->user, &user_name);
            strcpy(new_space_ptr->user, user_name);

            new_space_ptr->space_next = tmp_room_ptr->space_head;
            tmp_room_ptr->space_head = new_space_ptr;
            tmp_room_ptr->space_count++;
            printf("입주자 : %s\n", new_space_ptr->user);
            printf("%d \n", count);
        }
    }
    log_time();
    fprintf(fp, "[change_space] %dF %dRoom -> %dSpace : %s\n", target_floor, target_room, target_count);
}

void change_name(building *building_list) {
    char buff[256];
    int target_floor, target_room, target_count;
    printf("몇 층 입니까? 1 ~ %d 층\n", building_list->floor_count);
    if (tts_flag) {
        sprintf(buff, "pico2wave -w test.wav \"On a scale of 1 to %d, What is the floor number?\"",
                building_list->floor_count);
        system(buff);
        system("aplay -q test.wav");
        memset(buff, '\0', 256);
    }
    scanf("%d", &target_floor);
    floor_ptr tmp_floor_ptr = building_list->floor_head;
    int index = 0;
    for (index = 0; index < building_list->floor_count; index++) {
        if (target_floor == tmp_floor_ptr->floor_id) {
            break;
        }
        tmp_floor_ptr = tmp_floor_ptr->floor_next;
    }
    printf("몇 호 입니까? 1 ~ %d 호\n", tmp_floor_ptr->room_count);
    if (tts_flag) {
        sprintf(buff, "pico2wave -w test.wav \"On a scale of 1 to %d, What is the room number?\"",
                tmp_floor_ptr->room_count);
        system(buff);
        system("aplay -q test.wav");
        memset(buff, '\0', 256);
    }
    scanf("%d", &target_room);
    room_ptr tmp_room_ptr = tmp_floor_ptr->room_head;
    for (index = 0; index < tmp_floor_ptr->room_count; index++) {
        if (target_room == tmp_room_ptr->room_id) {
            break;
        }
        tmp_room_ptr = tmp_room_ptr->room_next;
    }

    while (1) {
        printf("몇 칸 입니까? 1 ~ 4 칸 [현재] : %d 칸 \n", tmp_room_ptr->space_count);
        if (tts_flag) {

            sprintf(buff, "pico2wave -w test.wav \"On a scale of 1 to %d, How many sections the room has?\"",
                    tmp_room_ptr->space_count);
            system(buff);
            system("aplay -q test.wav");
            memset(buff, '\0', 256);


        }
        scanf("%d", &target_count);
        if (target_count >= 1 && target_count <= 4) {
            break;
        } else {
            printf("다시 입력 해주세요.\n");
            if (tts_flag) {
                system("pico2wave -w test.wav \"Input the resident's name.\"");
                system("aplay -q test.wav");
            }
        }
    }
    char *user_name;
    int count;
    space_ptr tmp_space_ptr = tmp_room_ptr->space_head;
    for (count = 0; count < tmp_room_ptr->space_count; count++) {
        if ((count + 1) == target_count) {
            
            printf("입주자 이름을 입력하세요.\n");
            if (tts_flag) {
                system("pico2wave -w test.wav \"Input the resident's name.\"");
                system("aplay -q test.wav");
            }
            scanf("%s", &user_name);
            strcpy(tmp_space_ptr->user, &user_name);
        }
        tmp_space_ptr = tmp_space_ptr->space_next;
    }
    log_time();
    fprintf(fp, "[change_name] %dF %dRoom -> %dSpace : %s\n", target_floor, target_room, target_count, &user_name);
}

void search_name(building *building_list) {
    char *user_name;
    printf("입주자 이름을 입력하세요.\n");
    if (tts_flag) {
        system("pico2wave -w test.wav \"Input the resident's name.\"");
        system("aplay -q test.wav");
    }
    scanf("%s", &user_name);

    int index_floor, index_room, index_space;
    floor_ptr tmp_floor_ptr = building_list->floor_head;
    for (index_floor = 0; index_floor < building_list->floor_count; index_floor++) {
        room_ptr tmp_room_ptr = tmp_floor_ptr->room_head;
        for (index_room = 0; index_room < tmp_floor_ptr->room_count; index_room++) {
            space_ptr tmp_space_ptr = tmp_room_ptr->space_head;
            for (index_space = 0; index_space < tmp_room_ptr->space_count; index_space++) {
                if (strcmp(&user_name, tmp_space_ptr->user) == 0)
                    printf("%d층 %d호 %d칸 : %s\n", tmp_floor_ptr->floor_id, tmp_room_ptr->room_id, (index_space + 1),
                           tmp_space_ptr->user);
                tmp_space_ptr = tmp_space_ptr->space_next;
            }
            tmp_room_ptr = tmp_room_ptr->room_next;
        }
        tmp_floor_ptr = tmp_floor_ptr->floor_next;
    }
    log_time();
    fprintf(fp, "[search_name] user_name : %s\n", &user_name);
}


int main() {
    fp = fopen("log.txt", "w");
    log_time();
    fprintf(fp, "Start Programm\n");

    int target_floor, target_room;
    int view_flag = 1;
    building *building_list = (building *) malloc(sizeof(building));
    log_time();
    fprintf(fp, "init building_list\n");
    init(building_list);
    add_floor(building_list, FALSE);
    add_room(building_list, FALSE);
    add_room(building_list, FALSE);
    add_room(building_list, FALSE);

    while (1) {
        if (view_flag)
            view_building(building_list, TRUE);
        else {
            view_room(building_list, target_floor, target_room);
        }
        switch (input_cmd()) {
            case 1: { // [1] 건물 확장
                switch (input_extend_cmd()) {
                    case 1: { // [1] 층 확장
                        add_floor(building_list, TRUE);
                        break;
                    }
                    case 2: { // [2] 방 확장
                        add_room(building_list, TRUE);
                        break;
                    }
                }
                break;
            }
            case 2: { // [2] View 변환
                if (view_flag) {
                    printf("특정 방 내부 보여주기 로 View가 변환되었습니다.\n");
                    printf("층과 호수를 입력하세요\n");
                    if (tts_flag) {
                        system("pico2wave -w test.wav \"Translated the view into the specific room viewing.\"");
                        system("aplay -q test.wav");
                        system("pico2wave -w test.wav \"Input floor number and room number\"");
                        system("aplay -q test.wav");
                    }
                    scanf("%d %d", &target_floor, &target_room);
                    view_flag = 0;
                } else {
                    printf("전체 방 보여주기 로 View가 변환되었습니다.\n");
                    if (tts_flag) {
                        system("pico2wave -w test.wav \"Translated the view into the total room viewing.\"");
                        system("aplay -q test.wav");
                    }
                    view_flag = 1;
                }
                break;
            }
            case 3: { // [3] 방 별 칸 변환
                change_space(building_list);
                break;
            }
            case 4: { // [4] 칸 별 사람 이름 변환
                change_name(building_list);
                break;
            }
            case 5: {
                search_name(building_list);
                break;
            }
            case 6: { // [6] TTS ON/OFF
                if (tts_flag) {
                    printf("TTS를 사용하지않습니다.\n");
                    tts_flag = 0;
                } else {
                    printf("TTS를 사용합니다.\n");
                    system("pico2wave -w test.wav \"From now, Use text to speach.\"");
                    system("aplay -q test.wav");
                    tts_flag = 1;
                }
                break;
            }
            case 7: { // [5] 종료
                printf("\n프로그램을 종료합니다.\n");
                if (tts_flag) {
                    system("pico2wave -w test.wav \"Exit Program.\"");
                    system("aplay -q test.wav");
                }
                return 0;
                break;
            }
        }
    }
    return 0;
}


/*

o 단층에 방 3개 짜리 건물로 시작하여 높고 넓은 건물을 만드는 것을 목표로 한다.
o 건물은 다음과 같은 성질을 가진다.
o - 건물은 층을 늘릴 수도, 방을 늘리는 것도 가능하다.
o - 무한히 늘릴 수 있다.
o 방은 다음과 같은 성질을 가진다.
o - 각 방의 호수(번호)를 가진다.
o - 하나의 방은 최대 4칸까지 쪼개질 수 있다.
o - 한 칸에 한 사람 씩 살 수 있다.
o - 각 방에는 1인 이상 살고 있어야 한다.
o 화면에 표시되어야 하는 정보는 다음과 같다.
o - 현재 방의 숫자
o - 현재 건물에 살고 있는 사람의 수
o - 전체 방 보여주기 (건물 전체 View)
o - 특정 방 내부 보여주기 (방 내부 View)
o 입력을 통해 수행되어야 하는 기능은 다음과 같다.
o - 건물 확장 (층, 칸)
o - View 변환 (건물 보기, 특정 방 보기)
o - 방 별 칸 변환 (1~4개)
o - 칸 별 사람 이름 변환
o 사는 사람 위치 검색
o 로그 기록
o TTS(Text to Speach)

*/
