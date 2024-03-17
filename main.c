#include "video_decoder.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{ 
    const char* filename = "/home/tyh/桌面/2024春工程方向春招题/参考视频/dragon.mp4";
    int result = decoder_init(filename);
    int total_frames = 0;
    int height = 0,width = 0,linesize = 0;
    unsigned char red = 0,green = 0,blue = 0;
    double gray = 0;

    total_frames = get_total_frames(filename);
    struct _Frame frame_dragon[total_frames - 1];
    for (int i = 0;i <= total_frames - 1;i++){
        frame_dragon[i] = decoder_get_frame(filename);
    }
    

    height = frame_dragon[0].height;
    width = frame_dragon[0].width;
    //printf("%d",height);
    //printf("%d",width);
    unsigned char* data1 = frame_dragon[0].data;
        

    for (int y = 0;y < height;y++){
        for (int x = 0;x < width;x++){
            data1 += 3;
            red = *data1;
            green = *(data1 + 1);
            blue = *(data1 + 2);
            //gray = 0.2126 * red + 0.7152 * green + 0.0722 * blue;
            printf("\x1b[38;2;%d;%d;%dm█", red, green, blue);
        }
        printf("\x1b[0m\n");
    }
    decoder_close(filename);
    //printf ("%d\n%d\n%d\n",frame_dragon[1].height,frame_dragon[1].width,frame_dragon[1].linesize);
    //灰度值公式 gray ＝ 0.2126 * r + 0.7152 * g + 0.0722 * b

}
