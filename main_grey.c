#include "video_decoder.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{ 
    const char* filename = "/home/tyh/桌面/2024春工程方向春招题/参考视频/dragon.mp4";
    int result = decoder_init(filename);
    int total_frames = 0;
    int height = 0, width = 0;
    unsigned char* data = NULL;

    total_frames = get_total_frames(filename);
    struct _Frame frame_dragon[total_frames - 1];
    for (int i = 0; i < total_frames - 1; i++) {
        frame_dragon[i] = decoder_get_frame(filename);
    }
    
    height = frame_dragon[0].height;
    width = frame_dragon[0].width;
    data = frame_dragon[0].data;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char red = *data;
            unsigned char green = *(data + 1);
            unsigned char blue = *(data + 2);
            double gray = 0.2126 * red + 0.7152 * green + 0.0722 * blue;
            // 根据灰度值选择合适的 ASCII 字符表示像素
            if (gray < 64) {
                printf(" "); // 比较暗的灰度使用空格表示
            } else if (gray < 128) {
                printf("."); // 中等灰度使用点号表示
            } else if (gray < 192) {
                printf(":"); // 较亮的灰度使用冒号表示
            } else {
                printf("#"); // 最亮的灰度使用井号表示
            }
            data += 3; // 移动到下一个像素
        }
        printf("\n"); // 换行
    }
    decoder_close(filename);
}
