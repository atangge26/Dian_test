#include "video_decoder.h"
#include <stdio.h>
#include <stdlib.h>
Frame resize(Frame frame, int target_width, int target_height);

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
    Frame frame_resize = resize(frame_dragon[0],213,120);
    height = frame_resize.height;
    width = frame_resize.width;
    data = frame_resize.data;
    //printf("%d %d",width,height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char red = *data;
            unsigned char green = *(data + 1);
            unsigned char blue = *(data + 2);
            double gray = 0.2126 * red + 0.7152 * green + 0.0722 * blue;
            
            if (gray < 37){
                printf(" "); 
            }else if(gray < 73){
                printf("."); 
            }else if(gray < 110){
                printf(":"); 
            }else if(gray < 146){
                printf("="); 
            }else if(gray < 182){
                printf("#");
            }else if(gray < 219){
                printf("&");  
            }else{
                printf("@");
            }         
            data += 3; 
        }
        printf("\n"); 
    }
    decoder_close(filename);
    free(frame_resize.data);
}

Frame resize(Frame frame, int target_width, int target_height){
    Frame frame_resize;
    int height = frame.height;
    int width = frame.width;
    int linesize = frame.linesize;
    int x2 = frame.width/target_width;
    int y2 = frame.height/target_height;
    int num = x2*y2;
    unsigned char* data = frame.data;
    frame_resize.data = (unsigned char *)malloc(target_height*target_width*3*sizeof(unsigned char));
    for (int y = 0;y < height;y += y2){
        for (int x = 0;x < width;x += x2){
            double red_total = 0;            
            double green_total = 0;           
            double blue_total = 0;           
            for (int i = 0;i < y2;i++){
                for (int j = 0;j < x2;j++){
                    data = frame.data + linesize*(y + i) + 3*(x + j);
                    red_total += *data;
                    green_total += *(data + 1);
                    blue_total += *(data + 2);
                }
            }
            unsigned char red = (unsigned char)(red_total / num);         
            unsigned char green = (unsigned char)(green_total / num);      
            unsigned char blue = (unsigned char)(blue_total / num);       
            frame_resize.data[(x/x2)*3 + (y/y2)*target_width*3] = red;
            frame_resize.data[(x/x2)*3 + (y/y2)*target_width*3 + 1] = green;
            frame_resize.data[(x/x2)*3 + (y/y2)*target_width*3 + 2] = blue;
        }

    }
    frame_resize.height = target_height;
    frame_resize.width = target_width;
    frame_resize.linesize = target_width*3;
    return frame_resize;
}