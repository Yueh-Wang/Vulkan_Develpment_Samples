#include "BallData.h"
#include <vector>
#include <math.h>
#include <string.h>

float* BallData::vdata;
int BallData::dataByteCount;
int BallData::vCount;
float toRadians(float degree)
{
    return degree*3.1415926535898/180;
}
void  BallData::genBallData(float angleSpan)
{
    const float r=1.0f;
    std::vector<float> alVertix;
    for (int vAngle = -90; vAngle < 90; vAngle = vAngle + angleSpan)
    {
        for (int hAngle = 0; hAngle <= 360; hAngle = hAngle + angleSpan)
        {
            float x0 = (float) (r * cos(toRadians(vAngle)) * cos(toRadians(hAngle)));
            float y0 = (float) (r * cos(toRadians(vAngle)) * sin(toRadians(hAngle)));
            float z0 = (float) (r *sin(toRadians(vAngle)));
            float x1 = (float) (r * cos(toRadians(vAngle)) * cos(toRadians(hAngle + angleSpan)));
            float y1 = (float) (r * cos(toRadians(vAngle)) *sin(toRadians(hAngle + angleSpan)));
            float z1 = (float) (r * sin(toRadians(vAngle)));
            float x2 = (float) (r * cos(toRadians(vAngle + angleSpan)) * cos(toRadians(hAngle + angleSpan)));
            float y2 = (float) (r * cos(toRadians(vAngle + angleSpan)) * sin(toRadians(hAngle + angleSpan)));
            float z2 = (float) (r * sin(toRadians(vAngle + angleSpan)));
            float x3 = (float) (r * cos(toRadians(vAngle + angleSpan)) * cos(toRadians(hAngle)));
            float y3 = (float) (r * cos(toRadians(vAngle + angleSpan)) * sin(toRadians(hAngle)));
            float z3 = (float) (r * sin(toRadians(vAngle + angleSpan)));
            
            alVertix.push_back(x1);
            alVertix.push_back(y1);
            alVertix.push_back(z1);
            alVertix.push_back(x3);
            alVertix.push_back(y3);
            alVertix.push_back(z3);
            alVertix.push_back(x0);
            alVertix.push_back(y0);
            alVertix.push_back(z0);
            alVertix.push_back(x1);
            alVertix.push_back(y1);
            alVertix.push_back(z1);
            alVertix.push_back(x2);
            alVertix.push_back(y2);
            alVertix.push_back(z2);
            alVertix.push_back(x3);
            alVertix.push_back(y3);
            alVertix.push_back(z3);
        }
    }
    vCount = alVertix.size() / 3;//顶点的数量为坐标值数量的1/3，因为一个顶点有3个坐标分量
    dataByteCount=alVertix.size()*2* sizeof(float);//计算顶点和法向量数据总字节数
    vdata=new float[alVertix.size()*2];//创建存放顶点和法向量数据的数组
    int index=0;//辅助索引
    for(int i=0;i<vCount;i++)
    {
        vdata[index++]=alVertix[i*3+0];//保存顶点位置X分量
        vdata[index++]=alVertix[i*3+1];//保存顶点位置Y分量
        vdata[index++]=alVertix[i*3+2];//保存顶点位置Z分量

        vdata[index++]=alVertix[i*3+0]/r;//保存法向量X轴分量（除以r是为了规格化）
        vdata[index++]=alVertix[i*3+1]/r;//保存法向量Y轴分量（除以r是为了规格化）
        vdata[index++]=alVertix[i*3+2]/r;//保存法向量Z轴分量（除以r是为了规格化）
    }
}