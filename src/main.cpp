#include <iostream>
#include <vector>
using namespace std;

// WorkBench 工作台类
struct WorkBench
{
    int type;                 // 工作台类型
    float coordinate_x;       // 坐标  x
    float coordinate_y;       // 坐标 y
    int reaminsProductFrames; // 剩余生产时间
    int materials;            // 原材料格状态
    bool product_status;      // 产品格状态
};

struct Robot
{
    int workbench_id;             // 所处工作台ID
    int items_type;               // 携带物品类型
    float coefficientOfTimeValue; // 时间价值系数
    float collisionValueFactor;   // 碰撞价值系数
    float angularVelocity;        // 角速度
    float linearVelocity_x;         // 线速度 x
    float linearVelocity_y;         // 线速度 y
    float orientation;            // 朝向
    float coordinate_x;           // 坐标 x
    float coordinate_y;           // 坐标 y
};

bool readUntilOK()
{
    char line[1024];
    while (fgets(line, sizeof line, stdin))
    {
        if (line[0] == 'O' && line[1] == 'K')
        {
            return true;
        }
        // do something
    }
    return false;
}

bool ReadFrame()
{
    int frameId;
    int money;
    cin >> frameId >> money;
    int K;
    cin >> K;
    fprintf(stderr, "frameId = %d, money = %d, K = %d\n", frameId, money, K);

    return 1;
}

// int main() {
//     readUntilOK();
//     puts("OK");
//     fflush(stdout);
//     int frameID;
//     while (scanf("%d", &frameID) != EOF) {
//         readUntilOK();
//         printf("%d\n", frameID);
//         int lineSpeed = 3;
//         double angleSpeed = 1.5;
//         for(int robotId = 0; robotId < 4; robotId++){
//             fprintf(stderr,"forward %d %d\n", robotId, lineSpeed);
//             fprintf(stderr,"rotate %d %f\n", robotId, angleSpeed);
//         }
//         printf("OK\n", frameID);
//         fflush(stdout);
//     }
//     return 0;
// }

int main()
{
    readUntilOK();
    puts("OK");
    fflush(stdout);
    int frameID;
    while (scanf("%d", &frameID) != EOF)
    {
        // readUntilOK();
        // 读取数据
        cin.get();
        int money, K;
        cin >> money;
        cin.get();
        cin >> K;
        cin.get();
        // fprintf(stderr, "%d, %d, %d\n", frameID, money, K);
        vector<WorkBench> WorkBenchs;

        for (int i = 0; i < K; i++)
        {
            WorkBench w;
            cin >> w.type>>w.coordinate_x>>w.coordinate_y>>
                w.reaminsProductFrames>>w.materials>>w.product_status;
            cin.get();
            // fprintf(stderr, "stderr workbench: %d, %f,%f, %d,%d,%d \n",
            //         w.type, w.coordinate_x, w.coordinate_y, w.reaminsProductFrames,
            //         w.materials, w.product_status);
            
        }

        vector<Robot> Robots;
        // 读取机器人数据
        for(int i = 0; i<4;i++){
            Robot r;
            cin>>r.workbench_id>>r.items_type>>r.coefficientOfTimeValue>>
                r.collisionValueFactor>>r.angularVelocity>>r.linearVelocity_x>>
                r.linearVelocity_y>>r.orientation>>r.coordinate_x>>r.coordinate_y;
            cin.get();
            
            Robots.push_back(r);
        }

        // for(int i = 0; i<4; i++){
        //     Robot r = Robots[i];
        //     fprintf(stderr, "stderr Robot: %d,%d,%f,%f, %f,%f,%f,%f,%f,%f \n",
        //             r.workbench_id, r.items_type,r.coefficientOfTimeValue,
        //                 r.collisionValueFactor,r.angularVelocity,r.linearVelocity_x,
        //                 r.linearVelocity_y,r.orientation,r.coordinate_x,r.coordinate_y);

        // }
        char frameStatus[2];        // 记录是否为最后一帧，OK表示后面还有帧
        scanf("%s", frameStatus);
        fprintf(stderr, "%s",frameStatus);

        fflush(stdout);
    }
    return 0;
}
