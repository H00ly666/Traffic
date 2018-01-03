/*************************************************************************
	> File Name: sql.cpp
	> Author: 刘怪怪
	> Mail: 
	> Created Time: 2018年01月03日 星期三 11时50分30秒
 ************************************************************************/

#include<iostream>
#include<mysql/mysql.h>
using namespace std;
int main()
{
//    char sql[20]; // 执行语句
    int  i,j;
    int   arcs[3][3];
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
                for(j = 0 ;j< 3 ; j++){
                    arcs[i][j] = atoi(result_row[j]) ;   


                }
            cout << endl;
            }
        
        }
    
    }    
    for(i = 0; i< 2; i++){
        for(j = 0; j<3 ;j++)
            cout <<arcs[i][j] <<" ";
        cout<< endl;
    
    }
    mysql_close(conn);
    return 0 ; 
} 
