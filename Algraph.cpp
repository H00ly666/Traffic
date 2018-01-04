#include<iostream>
#include<fstream>
#include<string>
#include<queue>
#include<stack>
#include<vector>
#include"./login.cpp"
#include<mysql/mysql.h>
// #include"./login.cpp"
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
    MYSQL *conn;//一个sql套接字
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
    void Dijkstra(int start,int end, int dist[],int path[][50]);
    //查询最短路线
    void  Short_way();
    //采用DFS算法求得两点直接之间所有路径
    void  all_path() ;  //所有简单路径
    void DFS_All_ALGraph(int ,int );
    //采用BFS算法求得最少中转
    void BFS_ALGraph();
    //采用prim 算法求得最短联通线路图
    void Prim();
    //查询某个城市的最短联通图
    void MiniSpanTree();
    void  Save();
    
    void  menu( int flag );    //选项菜单
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
    //mysql_close(conn);
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
                cout << vex[i].name <<"<---->"<< vex[j].name<< arcs[i][j]<<"Km"<<endl ;
        }
}

/**********************
    显示城市信息（）
    void Display_City()
**********************/
void  ALGraph:: Display_City()
{
    for(int i=1 ;i<=vexnum ;i++ ){
        cout <<i << ":  " << vex[i].name <<endl;
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
    
    char sql[100];
    sprintf(sql,"insert into arcs (`from`, `to` , `length`) VALUES (%d ,%d , %d) ",start ,end,length );
    
    cout << sql <<endl;
    
    int res = mysql_query (conn, sql);  //正确返回0
    if(res){
        cout << mysql_error(conn) << endl;
        cout <<"error" <<endl;
        mysql_close(conn);
        //exit(0);
        return ;
    }
    
    sprintf(sql,"insert into arcs (`from` , `to` , `length`) VALUES (%d ,%d ,%d) ", end,start,length);
    res = mysql_query (conn, sql);  //正确返回0
    if(res){
        cout <<"error2" <<endl;
        mysql_close(conn);
        exit(0);
    }
    Creat();
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
    


    char sql[100];
    sprintf(sql,"delete from  arcs  where  `from` = %d and `to` = %d",start ,end);

    cout << sql <<endl;
    
    int res = mysql_query (conn, sql);  //正确返回0
    if(res){
        cout << mysql_error(conn) << endl;
        cout <<"error" <<endl;
        mysql_close(conn);
        exit(0);
        return ;
    }
    
    sprintf(sql,"delete from  arcs  where  `from` = %d and `to` = %d",end ,start );
    res = mysql_query (conn, sql);  //正确返回0
    if(res){
        cout <<"error2" <<endl;
        mysql_close(conn);
        exit(0);
    }
    Creat();
}

/**********************
    采用Dijkstra算法求得从起点城市到各个城市的最短路径
    一次性解决所有最短路径
    void Dijkstra(int start,int end, int dist[],int path[][MXVEX])

**********************/

void ALGraph::  Dijkstra(int start,int end, int dist[],int path[][50])
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
    if (dist[end] == INFINITY)
    {
        cout << "不存在的" <<endl;
        return ;
    }
        
        cout << vex[start].name << "<--->"<< vex[end].name << "的最短线路为:从 " << vex[start].name;
    
    for(j =2; path[end][j]!= 0 ;j++)
        cout<< "->" << vex[path[end][j]] .name;
    cout << "->" << vex[end].name <<"距离为: " <<dist[end]<<"KM" <<endl ;
    
}

/**********************
    查询最短路线
    void  Short_way()
**********************/
void ALGraph:: Short_way()
{
    string city;
    int start,end;
    int dist[20],path[50][50] = {0};
    cout<<"请输入起始城市： ";
    cin >> city;
    start = Locate(city);
    cout<<"请输入终点城市： ";
    cin >> city;
    end  = Locate(city);
    Dijkstra(start,end, dist, path);

}

/**********************
    采用DFS算法求得两点直接之间所有路径
    void DFS_All_ALGraph()
**********************/
/*
void ALGraph::DFS_All_ALGraph()
{
    stack<int> S;  
    
    // book 用来标p记是否被访问过  初始化为0
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

    S.push(start);
    
    while ( !S.empty() ){
        v = S.pop();
        if(book[v] != 1 )
            cout << endl;
        //寻找v的第一个邻接点
        for(i = 1; i<=vexnum ;i++ ){
            if(arcs[S.top][i] !=INFINITY &&  book[i] ==0 ){        
                S.push(i);
                break;
                D
            }
        }
        
    }
   
}
*/
int book[100];
vector < int > V;
void ALGraph:: all_path()   //所有简单路径
{
    book[100] = {0};
    string str1,str2;
    int v1,v2;
    cout << "输入起始点：\n";
    cin >> str1;
    cout<<"请输入终点城市： ";
    cin >> str2;

    v1 =Locate( str1 );
    if( v1 == -1 )
    {
        cout << "无此地点\n";
        return ;
    }
    v2 = Locate( str2 );
    if( v2 == -1 )
    {
        cout << "无此地点\n";
        return ;
    }
    DFS_All_ALGraph( v1,v2 );

}
//void ALGraph :: dfs_all( int v1,int v2 )
void ALGraph:: DFS_All_ALGraph(int v1 ,int v2 )
{
    int i,j;
    V.push_back( v1 );
    book[ v1 ] = 1;
    if( v1 == v2 )
    {
        for( int i : V )
        {
            cout << vex[i].name << ' ';
        }
        cout << endl;
    }
    else 
    {
        for( i = 1;i <= vexnum;i++ )
        {
            if( arcs[ v1 ][i] != INFINITY && book[i] == 0 )
            {
                DFS_All_ALGraph( i,v2 );
            }
        }
    }
    book[ v1 ] = 0;
    v1--;
    V.pop_back();

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
    // book 用来标p记是否被访问过  初始化为0
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
    cout << "路径为 " <<endl;
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
void ALGraph :: menu( int flag )    //选项菜单
{
    int choose;
    cout << "0.退出\n"
         << "1.显示地图所有地点\n"
         << "2.查看两点间最短路径\n"
         << "3.查看两点间转机最少的路径\n"
         << "4.查看两点间所有简单路径\n"
         << "5.增加路径\n"
         << "6.删除路径\n"
         << "7.显示所有路径\n";   
    cout << "输入选项：" << endl; 
    cin >> choose;
    while( choose != 0 )
    {
        if( choose == 1 )
        {
            Display_City();

        }
        if( choose == 2 )
        {
            Short_way();
        }
        if( choose == 3 )
        {
            BFS_ALGraph();
        }
        if( choose == 4 )
        {
            all_path();
        }
        if( choose == 5 )
        {
            Add_way();
        }
        if( choose == 6 )
        {
            if( flag != 1 )
            {
                cout << "缺少权限" << endl;
            }
            else
            {
                Add_way();
            }
        }
        if( choose == 6 )
        {
            if( flag != 1 )
            {
                cout << "缺少权限" << endl;
            }
            else
            {
                Delete_way();
            }
        
        }
        if(choose == 7)
        {
            Display();
        }
        cout << '\n' << "输入选项:" << endl;
        cin >> choose;
    }
}
int main ()
{
    ALGraph  G;    
    G.Creat();
   // G.Display();
   // G.all_path();
   // G.Add_way();
   // G.Display();
    if(start() ){
        G.menu(1);
    } 


}


