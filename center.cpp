//
// Created by Centauria V. CHEN on 2023/3/10.
//

#include <cassert>
#include <cstring>
#include <iostream>
#include <set>
#include <vector>

#include "center.h"
#include "point.h"


Center::Center()
{
    robots = std::vector<std::unique_ptr<Robot>>();
    workbenches = std::vector<std::unique_ptr<WorkBench>>();
    for (int i = 0; i < 4; i++)
    {
        robots_goal[i].giver_id = -10;
        robots_goal[i].receiver_id = -10;
        robots_goal[i].item_type = -10;
    }
    memset(item_occur_cnt, 0, sizeof(item_occur_cnt));
}

void Center::set_adj_matrix(std::vector<Point> &workbench_position)
{
    memset(adj_matrix, 0, sizeof(adj_matrix));
    for(int i = 0; i < workbench_position.size(); i++){
        for(int j = i; j < workbench_position.size(); j++){
            if(i == j){
                adj_matrix[i][j] = 0;
                continue;
            }
            double dx = workbench_position[i].x - workbench_position[j].x;
            double dy = workbench_position[i].y - workbench_position[j].y;
            adj_matrix[i][j] = sqrt(dx*dx + dy*dy);
            adj_matrix[j][i] = sqrt(dx*dx + dy*dy);
        }
    }
}

void Center::initialize()
{
    char line[1024];
    int16_t robot_num = 0;
    const int maps_row_num = 100;
    const int maps_col_num = 100;
    int i = 0;
    // 从地图的左上角开始， i 表示行数，j表示列数
    // 假定地图数据为100*100的
    std::vector<Point> workbench_position;
    // sorry 我的直觉告诉我，上面这两个vector 应该没有必要声明。你应该已经写过了，就是底下那个东西的，但我不太会用，所以就自己又写了一个
    while (fgets(line, sizeof line, stdin) && i < maps_row_num)
    {
        if (line[0] == 'O' && line[1] == 'K') break;
        // 判断每一行的信息
        for (int j = 0; j < maps_col_num; j++)
        {
            if (line[j] >= '1' && line[j] <= '9')
            {
                workbench_position.push_back({0.25 + 0.5 * j, 49.75 - 0.5 * i});
                auto type = int16_t(line[j] - '0');
                workbenches.emplace_back(std::make_unique<WorkBench>(WorkBench{type, 0.25 + 0.5 * j, 49.75 - 0.5 * i}));

            } else if (line[j] == 'A')
            {
                robots.emplace_back(std::make_unique<Robot>(Robot{robot_num, 0.25 + 0.5 * j, 49.75 - 0.5 * i}));
                robots.back()->_logging_name = "robot_" + std::to_string(robot_num);
                robot_num++;
            }
        }
        i++;
    }
    set_adj_matrix(workbench_position);
    std::cout << "OK" << std::endl;
    std::flush(std::cout);
    workbench_position.clear();
}
bool Center::refresh()
{
    int frameID;
    std::cin >> frameID;
    if (frameID == EOF) return false;
    deltaFrame = frameID - currentFrame;
    currentFrame = frameID;
    int money;
    std::cin >> money;
    std::cin.get();
    int workbench_count;
    std::cin >> workbench_count;
    assert(workbench_count == workbenches.size());
    int16_t workbench_id_counter = 0;
    for (auto &workbench: workbenches)
    {
        std::cin.get();
        std::cin >> workbench->type >> workbench->coordinate.x >> workbench->coordinate.y >> workbench->product_frames_remained >> workbench->material_status >> workbench->product_status;
        workbench->id = workbench_id_counter;
        workbench_id_counter++;
    }

    for (auto &robot: robots)
    {
        std::cin.get();
        std::cin >> robot->workbench_id >> robot->item_type >> robot->time_val >> robot->collision_val >> robot->omega >> robot->velocity.x >> robot->velocity.y >> robot->orientation >> robot->coordinate.x >> robot->coordinate.y;
    }

    std::string ok;
    std::cin >> ok;
    assert(ok == "OK");

    return true;
}
void Center::step()
{
    std::cout << currentFrame << std::endl;
    for (auto &robot: robots)
    {
        robot->step(deltaFrame / frameRate);
        if (robot->item_type >= 1)
        {
            if (robot->workbench_id == robots_goal[robot->id].receiver_id)
            {
                robot->sell();
                item_occur_cnt[robot->item_type]++;
                robots_goal[robot->id].receiver_id = -10;
                robots_goal[robot->id].status = false;
            }
        } else
        {
            if (robot->workbench_id == robots_goal[robot->id].giver_id)
            {
                robot->buy();
                robots_goal[robot->id].giver_id = -10;
            }
        }
    }
    std::cout << "OK" << std::endl;
    LOG("logs/behavior.log", "OK")
    std::flush(std::cout);
}
void Center::decide()
{
    // TODO: Set target for every robot
    // By adjusting the variable "target" I can control the movement of the robot
    // Since "target" is a private variable
    // I should call "set_target" to change its value!
    for (auto &robot: robots)
    {
        if (!robots_goal[robot->id].status) // have task or not
        {
            if(get_Task(robot->id))
                robots_goal[robot->id].status = true;
        }
        if (robot->item_type >= 1)
        {
            robot->set_target(robots_goal[robot->id].receiver_point);
        } else
        {
            robot->set_target(robots_goal[robot->id].giver_point);
        }
        std::vector<Point> obstacles;
        for (auto &r: robots)
        {
            if (r != robot)
            {
                obstacles.emplace_back(r->coordinate);
            }
        }
        robot->set_obstacle(obstacles);
    }
    return;
}

void Center::UpdateSupply()
{
    std::set<int> supply_check;
    // 用set来记录robot在做的任务给予者位置，避免重复领取item
    for (int i = 0; i < 4; i++)
        supply_check.insert(robots_goal[i].giver_id);

    for (int t = 7; t >= 1; t--)
    {
        for (auto &workbench: workbenches)
        {
            if (!workbench->product_status)
                continue;
            if (supply_check.count(workbench->id))
                continue;
            if (workbench->type != t)
                continue;
            Supply temp;
            temp.workbench_id = workbench->id;
            temp.workbrench_point.x = workbench->coordinate.x;
            temp.workbrench_point.y = workbench->coordinate.y;
            temp.workbrench_type = workbench->type;
            temp.item_type = workbench->type;
            supply_list[t].push(temp);
        }
    }
    supply_check.clear();
    return;
}

void Center::UpdateDemand()
{
    std::set<int> demand_check[15];
    for (int i = 0; i < 4; i++)
        demand_check[abs(robots_goal[i].item_type)].insert(robots_goal[i].receiver_id);
    for (int t = 7; t >= 1; t--)
    {
        for (auto &workbench: workbenches)
        {
            if (workbench->isFree(t))
                continue;
            if (demand_check[t].count(workbench->id))
                continue;
            if (!workbench->needRawMaterial(t))
                continue;
            Demand temp;
            temp.workbench_id = workbench->id;
            temp.workbrench_point.x = workbench->coordinate.x;
            temp.workbrench_point.y = workbench->coordinate.y;
            temp.workbrench_type = workbench->type;
            temp.item_type = t;
            demand_list[t].push_back(temp);
        }
    }
    for (int i = 0; i < 10; i++)
        demand_check[i].clear();
    return;
}

bool Center::get_Task(int robot_id)
{
    UpdateSupply();
    UpdateDemand();
    Task ans;
    int ans_dist = 9999;
    bool flag = false;
    for (int t = 7; t >= 1; t--)
    {
        if(!supply_list[t].empty()&&!demand_list[t].empty()){
            flag = true;
            while(supply_list[t].size()){
                Supply s = supply_list[t].front();
                supply_list[t].pop();
                for(auto d: demand_list[t]){
                    // robot->supply + supply->demand
                    int total_dist;
                    if(robots[robot_id]->workbench_id == -1){
                        int dx = robots[robot_id]->coordinate.x - s.workbrench_point.x;
                        int dy = robots[robot_id]->coordinate.y - s.workbrench_point.y;
                        total_dist = sqrt(dx*dx + dy*dy) + adj_matrix[s.workbench_id][d.workbench_id];
                    }
                    else{
                        total_dist = adj_matrix[robots[robot_id]->workbench_id][s.workbench_id] + adj_matrix[s.workbench_id][d.workbench_id];
                    }
                    if(total_dist < ans_dist){
                        ans.item_type = t;
                        //--------------------giver----------
                        ans.giver_id = s.workbench_id;
                        ans.giver_type = s.workbrench_type;
                        ans.giver_point.x = s.workbrench_point.x;
                        ans.giver_point.y = s.workbrench_point.y;
                        // ------------------receiver--------
                        ans.receiver_id = d.workbench_id;
                        ans.receiver_type = d.workbrench_type;
                        ans.receiver_point.x = d.workbrench_point.x;
                        ans.receiver_point.y = d.workbrench_point.y;
                        ans_dist = total_dist;
                    }
                }
            }
        }
        if((t == 7 || t == 4 || t == 1) && flag == true ){
            robots_goal[robot_id] = ans;
            // 如果能到这一步就说明至少得到一个答案 那么就return来表示已为机器人分发一个Task
            FreeSupplyDemandList();
            return true;
        }
    }
    FreeSupplyDemandList();
    return false;
}

void Center::FreeSupplyDemandList()
{
    for (int i = 7; i >= 1; i--)
    {
        while (supply_list[i].size())
        {
            supply_list[i].pop();
        }
        if (supply_list[i].size())
            std::cerr << "------------------------error-------------------" << std::endl;
    }
    for (int i = 7; i >= 1; i--)
    {
        while (demand_list[i].size())
        {
            demand_list[i].pop_back();
        }
        if (demand_list[i].size())
            std::cerr << "------------------------error-------------------" << std::endl;
    }
    return;
}