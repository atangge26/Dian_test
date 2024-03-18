#include "video_decoder.h"
#include <stdio.h>
#include <stdlib.h>


/*Frame* deposit(const char* filename){
    
    int total_frames = get_total_frames(filename);
    
    Frame* frame = (Frame*)malloc(total_frames*sizeof(Frame));
    for (int i = 0;i <= total_frames - 1;i++){
        frame[i] = decoder_get_frame();
    }
    //decoder_close(filename);
    return frame;
}*/


void print_rgb(Frame frame){
    int height = frame.height;
    int width = frame.width;
    //printf("%d ",height);
    //printf("%d",width);
    unsigned char* data1 = frame.data;
    //printf("\033[H");  
    for (int y = 0;y < height;y++){
        for (int x = 0;x < width;x++){
            unsigned char red = *data1;
            unsigned char green = *(data1 + 1);
            unsigned char blue = *(data1 + 2);
            data1 += 3;
            
            printf("\x1b[38;2;%d;%d;%dm█",red,green,blue);
        }
        printf("\x1b[0m\n");
    }
}


void print_grey(Frame frame){
    int height = frame.height;
    int width = frame.width;
    unsigned char* data = frame.data;
    //printf("%d %d",width,height);
    //printf("\033[H");
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


int main(){
    const char* filename = "/home/tyh/桌面/2024春工程方向春招题/参考视频/dragon.mp4";
    int target_width = 0,target_height = 0;
    char color = 'n';
    int result = decoder_init(filename);
     if (result != 0){
        printf("error!");
        return 0;
    }
    int total_frames = get_total_frames(); 

    printf ("color or grey? (print 'c' for color)");
    scanf ("%c",&color);
    printf ("请输入预期帧的宽度：");
    scanf ("%d",&target_width);
    printf ("请输入预期帧的高度：");
    scanf ("%d",&target_height);
    printf ("\n");

    //Frame* resize_frames = (Frame*)malloc(total_frames * sizeof(Frame));
    //Frame* origin_frames = (Frame*)malloc(total_frames * sizeof(Frame));
    
    for (int i = 0;i < 1000 ;i++){
        Frame origin_frame = decoder_get_frame();
        Frame resize_frame = resize(origin_frame,target_width,target_height);
        if (color == 'c'){
            print_rgb(resize_frame);
        }else{
            print_grey(resize_frame);
        }
        printf("\033[H\033[2J"); 
        printf("\033[1;1H"); 
        for (int j = 0;j < 10000000;j++){}
    }
    /*for(int i = 0;i < total_frames;i++){
        resize_frames[i] = resize(origin_frames[i], target_width,target_width);
    }
    clearScreen();
    for (int i = 0;i < 10;i++){
        print_picture(resize_frames[i]);
    }
    free(resize_frames);
    free(origin_frames);*/
    decoder_close(filename);
}