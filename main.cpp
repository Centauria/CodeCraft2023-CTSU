#include <iostream>
#include <string>
using namespace std;

bool readUntilOK()
{
    char line[1024];
    while (fgets(line, sizeof line, stdin))
    {
        if (line[0] == 'O' && line[1] == 'K')
        {
            return true;
        }
        //do something
    }
    return false;
}

struct Point {
    double x, y;
};
struct Vector {
    double x, y;
};

class Workbench
{
public:
    int type;                   // 工作台类型
    Point coordinate;           //坐标
    int product_frames_remained;// 剩余生产时间
    int material_status;        // 原材料格状态
    bool product_status;        // 产品格状态
};

class Robot
{
public:
    int workbench_id;          // 所处工作台id
    int item_type;             // 携带物品类型
    double time_val = 0.0;     // 时间价值系数
    double collision_val = 0.0;// 碰撞价值系数
    double omega = 0.0;        // 角速度
    Vector lineSpeed;          // 线速度
    double orientation;        // 朝向
    Point coordinate;          // 坐标
};

int main()
{
    readUntilOK();
    puts("OK");
    fflush(stdout);
    int frameID;
    while (scanf("%d", &frameID) != EOF)
    {
        int money, k;
        cin >> money;
        cin >> k;
        Workbench workbenches[k];
        for (auto &w: workbenches)
        {
            cin >> w.type >> w.coordinate.x >> w.coordinate.y >> w.product_frames_remained >> w.material_status >> w.product_status;
        }
        Robot robots[4];
        for (auto &r: robots)
        {
            cin >> r.workbench_id >> r.item_type >> r.time_val >> r.collision_val >> r.omega >> r.lineSpeed.x >> r.lineSpeed.y >> r.orientation >> r.coordinate.x >> r.coordinate.y;
        }
        //----------------👆inputs--------------
        string ok;
        cin >> ok;
        cout << frameID << endl;
        int lineSpeed = 5;
        double angleSpeed = 0;
        for (int robotId = 0; robotId < 4; robotId++)
        {
            printf("forward %d %d\n", robotId, lineSpeed);
            printf("rotate %d %f\n", robotId, angleSpeed);
        }
        cerr << robots[0].lineSpeed.x << endl;
        printf("OK\n");
        fflush(stdout);
    }
    return 0;
}
