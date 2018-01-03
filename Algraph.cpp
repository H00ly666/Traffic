#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<queue>
#include<mysql/mysql.h>
#define INFINITY 32768
using namespace std;
/*
    节点信息  代表某一城市
    1.城市信息
    2.城市编号
    3.
*/
struct City 
{   
    int num;
    string name;
    //string Introduction；
};
/*
    图信息   代表整个地图
    存边信息  ：票价 时间 距离 

*/
class ALGraph
{
    public:
    int  vexnum;        //顶点数目
    int  arcnum;         //边数目
    vector <City> vex;  //mZ
    vector <vector <int> > arcs; //边集
    vector <vector <int> > short_arcs;  //经过最短路径算法得到的结果存入其中
    //
    int Creat();
    //根据城市名确定城市序号
    int Locate(string& name);
    //显示图信息（）
    void Display();
    //显示城市信息（）   
    void   Display_City ();
    //查询图信息  A->B的情况
    void Search();
    //添加新路线
    void  Add_way();
    //删除某一条路线
    void Delete_way();
    //采用Dijkstra算法求得从起点城市到各个城市的最短路径
    void Dijkstra(int start,int end, int dist[],int path[][20]);
    //查询最短路线
    void  Short_way();
    //采用BFS算法求得最少中转
    void BFS_ALGraph();
    //采用prim 算法求得最短联通线路图
    void Prim();
    //查询某个城市的最短联通图
    void MiniSpanTree();
    void  Save();
};

/**********************
    登录
    1.用户登陆
        2.管理员登录
**********************/

/**********************
    从文件或数据库中获取整个图的信息（包含城市的的节点信息）
    int G_creat()   //    
**********************/
int ALGraph::Creat()
{
    int i, j,x,y,length;
    City  new_city;
    vector <int> lie;
    ifstream inFile("tu.txt", ios::in);
    //直接从文件中读取顶点数，边数
    inFile >> vexnum >> arcnum ;  
    //从中读取城市名,并将编号付给num;
    
    //给城市数组提前放一个  从一记  下标对应
    vex.push_back(new_city) ;
    for (int  i = 1;i <=vexnum ;i++){
        inFile >> new_city.name;
        new_city.num = i;  //从一开始存
        vex.push_back(new_city);
    }
    for (auto&h: vex )
        cout << h.num << " : " << h.name << endl;
    //××建立二维容器
    for(i=0;i<= arcnum ;i++){
        lie.push_back(INFINITY);
    }
    for(i=0;i<= arcnum ;i++){
        arcs.push_back(lie);
    }
    /*
    //从中读取边信息 并赋值给数组中           
    for(i=1;i<=(this->vexnum * this->vexnum) ;i++){
        inFile >> x >> y >>length;
        arcs[x][y] = length;
    }
    */
    char   sql[20] = "select * from arcs";
    int  res; // 执行sql语句返回的标志
    MYSQL_RES *res_ptr; //指向查询结果的指针
    MYSQL_FIELD *field; //字段结构指针
    MYSQL_ROW result_row; //按行返回查询信息
    int row,column;//返回查询到的行数和列数

    MYSQL *conn; //一个数据库连接指针
    //用NULL 来初始化链接句柄
    conn = mysql_init(NULL);
    if(conn == NULL){
        cout << "初始化失败"<< endl ;
        return  0;
    }
    //进行实际链接
    //参数 conn连接句柄 mysql所在的主机地址，user用户名,passwd 密码，database_name 数据库名, 后面都是默认参数
    conn = mysql_real_connect(conn,"localhost","root","liujiahui", "tu",0,NULL,0  );
    
    if (conn) {
        cout  << "connection success ! " << endl;
    }else{
        cout << "connection failed  !" <<endl;
    }
    mysql_query(conn , "set names gbk"); //防止乱码,设置和数据库一致就不会乱码 
    res = mysql_query (conn, sql);  //正确返回0
    if(res){
        cout <<"error" <<endl;
        mysql_close(conn);
        exit(0);
    } else{
        //把查询结果给res_ptr
        res_ptr = mysql_store_result(conn);
        //如果结果不为空,则输
        if(res_ptr){
            column =  mysql_num_fields(res_ptr);
            row    =  mysql_num_rows(res_ptr);
            cout << "查到"<< row <<  "行"<<column<< "列！" <<endl;
            //输出结果的字段名
            for( i = 0; i< row ;i++){
                result_row = mysql_fetch_row(res_ptr);
                arcs[atoi(result_row[0])][atoi(result_row[1])] = atoi(result_row[2]); //atoi将字符串转化成数字
            }     
        } 
    }    
    for(i = 1; i<= vexnum; i++){
        for(j = 1; j<= vexnum ;j++)
            cout <<arcs[i][j] <<" ";
        cout<< endl;
    
    }
    mysql_close(conn);
}

/**********************
    //根据城市名确定城市序号
    int Locate(string &name);
**********************/
int ALGraph::  Locate(string& name)
{
    for(auto& i: vex)
        if(i.name == name  )
            return i.num;
    return 0;
}

/**********************
    显示图信息（）
    void Display()
**********************/
void  ALGraph:: Display()
{
    cout<<"地图为" <<endl;
    for(int i=1;i<= vexnum;i++ )
        for(int  j=1;j<= vexnum;j++)
        {
            if( arcs[i][j] != INFINITY)
                cout << vex[i].name <<"<---->"<< vex[j].name<< arcs[i][j]<<"Km";
        }
}

/**********************
    显示城市信息（）
    void Display_City()
**********************/
void  ALGraph:: Display_City()
{
    for(auto&h : vex){
        cout <<h.num << ": "<< h.name <<endl;
    }

}
/**********************
    查询图信息  A->B的情况
    void search()
**********************/
void ALGraph:: Search()
{
    int  num,i,j ;
    string city;
    cin >> city;
    num =  Locate(city);
    if( !num ){
        cout<<"无" <<endl;
        return;
    }
    cout <<num << city <<"的交通图为" <<endl;
    for( j=1; j<=vexnum; j++)
        if( arcs[num][j] != INFINITY)
            cout << vex[num].name <<"<---->"<< vex[j].name<< arcs[num][j]<<"Km"<<endl;
}

/**********************
    添加新路线
    void  Add_way()
    只是增加一条边而已
***********************/  
void ALGraph:: Add_way()
{
    string  city;
    int  start ,end, length;
    cout<<"请输入起始城市： ";
    cin >> city;
    start = Locate(city);
    cout<<"请输入终点城市： ";
    cin >> city;
    end  = Locate(city);
    cout << "距离为: " ;
    cin >> length ;
     arcs[start][end] = length;
     arcs[end][start]  = length;
    cout << "添加成功"<<endl;
}

/**********************
    删除某一条路线
    void Delete_way(ALGraph &G)
**********************/
void ALGraph:: Delete_way()
{
    string  city;
    int  start ,end, length;
    cout<<"请输入起始城市： ";
    cin >> city;
    start = Locate(city);
    cout<<"请输入终点城市： ";
    cin >> city;
    end  = Locate(city);

     arcs[start][end] = INFINITY;
     arcs[end][start]  = INFINITY;
    cout << "删除成功"<<endl;

}

/**********************
    采用Dijkstra算法求得从起点城市到各个城市的最短路径
    一次性解决所有最短路径
    void Dijkstra(int start,int end, int dist[],int path[][MXVEX])

**********************/
void ALGraph::  Dijkstra(int start,int end, int dist[],int path[][20])
{
    int  mindist, i, j, k, t =1;
    for(i=1; i<= vexnum ;i++){
        dist[i] = arcs[start][i];
        if( arcs[start][i]!=INFINITY )
            path[i][1] = start;
    }
    path[start][0] = 1;
    
    for(j=1;j< vexnum;j++){
        mindist = INFINITY;
        for(j=1;j<= vexnum;j++){
            if(!path[j][0]&&dist[j]< mindist ){
                k = j;
                mindist = dist[j];
            }
        }
        
        if(mindist == INFINITY) break;
        path[k][0] = 1;
    
        for(j = 1; j<= vexnum;j++){
            if(!path[j][0] && arcs[k][j] <INFINITY && dist[k] + arcs[k][j] < dist[j]  ){
                dist[j] = dist[k] + arcs[k][j];
                t = 1;
                while(path[k][t] != 0 ){   //记录最新的最短路线
                    path[j][t] = path[k][t];
                    t++;
                }
                path[j][t] = k;
                path[j][t+1] = 0;
            }
        }
    }
    for(i=1;i <= vexnum;i++)
        if(i==end ) return;
    cout << vex[start].name << "<--->"<< vex[end].name << "的最短线路为:从 ";vex[start].name;
    
    for(j =2; path[i][j]!= 0 ;j++)
        cout<< "->" << vex[path[i][j]] .name;
    cout << "->" << vex[end].name <<"距离为: " <<dist[i]<<endl;
    
}
/**********************
    查询最短路线
    void  Short_way()
**********************/
void ALGraph::  Short_way()
{
    string city;
    int start,end;
    int dist[20],path[20][20] = {0};
    cout<<"请输入起始城市： ";
    cin >> city;
    start = Locate(city);
    cout<<"请输入终点城市： ";
    cin >> city;
    end  = Locate(city);
    Dijkstra(start,end, dist, path);


}

/**********************
    采用BFS算法求得最少中转
    void BFS_ALGraph()
**********************/
void ALGraph::BFS_ALGraph()
{
    //队列的类型
    struct X{
        int num;
        vector<int>  path;
    };
    
    X temp;
    //队列Q
    queue<X> Q; 
    // book 用来标记是否被访问过  初始化为0
    int  book[vexnum+1] = {0};

    //城市起点 终点
    string city;
    int start,end,flag=0 ,sum =1;
    cout<<"请输入起始城市： ";
    cin >> city;
    start = Locate(city);
    if(!start){
        cout <<"不存在的--"<< city << "请重新操作" <<endl;
        return;
    }
    cout<<"请输入终点城市： ";
    cin >> city;  
    end  = Locate(city);
    if(!start){
        cout <<"不存在的--"<< city << "请重新操作" <<endl;
        return;
    }
    
    temp.num = start;
    temp.path.push_back(start);
    Q.push(temp);
    
    vector<int> res;
    while (!Q.empty()){
        //队头出一个
        
        X del = Q.front(); //将出队的保存起来 
        Q.pop();
        book[del.num] = 1;
        if (del.num == end)
        {
            res = del.path;
            break;
        }
        for( int j = 1; j<= vexnum ;j++ ){
            //将于出队的临接的点进入队列之中
            if(arcs[del.num][j] != INFINITY && book[j] ==0 ){          
                 X temp;
                temp.num = j;
             //   cout << j  << " " << del.path.size() << endl; 
                temp.path = del.path;
                temp.path.push_back(j);

                //cout <<"TEMP 大小为" <<  temp.path.size() <<endl;
                //存入其中
                Q.push(temp);       
            }
        }
        sum++;
    }
    cout << "路将为" <<endl;
    for(auto &h: res){
        cout << h << " " <<endl;
    }
}
/**********************
    采用prim 算法求得最短联通线路图
    void Prim()
**********************/
void ALGraph:: Prim()
{

}
/**********************
    查询某个城市的最短联通图
    void MiniSpanTree()
    
**********************/
void ALGraph:: MiniSpanTree()
{

}
/**********************
    将文件全部保存下来
    void  Save()
**********************/
void ALGraph:: Save()
{

}
/**********************

**********************/

int main ()
{
    ALGraph  G;    
    G.Creat();
    G.Display();
    G.Display_City();
   // G.Search();
    G.BFS_ALGraph();
}

