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

typedef room *room_ptr; // Room Ponter

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
