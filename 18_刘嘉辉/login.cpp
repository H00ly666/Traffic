/*************************************************************************
	> File Name: login.cpp
	> Author: 刘怪怪
	> Mail: 
	> Created Time: 2018年01月04日 星期四 11时01分30秒
 **a**********************************************************************/
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<mysql/mysql.h>
using namespace std;

//清空缓冲区问题
inline int  chose_mode();

//getch 
inline char getch();
//暗文输入密码 
inline int  sign_in_register(int mode);

inline int start();
//
inline void get_passd(char *passd);

//清空缓冲区问题
inline  int  chose_mode();
//从数据库中查询
inline int sql_mima(char *name, char  *passd);


/*************
 *返回 0 为登录失败
  返回 1 为普通用户登录
  返回 2 为管理员用户登录
 ************/



int start()
{
   // system("clear");
    int  type;
    printf("\n\n\n\n\n\n\t\t\t\t-------------------------------------------------------\n");
    printf("\t\t\t\t-------------------------------------------------------\n");
    printf("\t\t\t\t-------------------------------------------------------\n");
    printf("\t\t\t\t------------------------德高地图-----------------------\n");
    printf("\t\t\t\t-----------------------1.登录--------------------------\n");
    printf("\t\t\t\t-----------------------2.注册--------------------------\n");
    printf("\t\t\t\t-----------------------0.退出--------------------------\n");
    printf("\t\t\t\t-------------------------------------------------------\n");
    printf("\t\t\t\t-------------------------------------------------------\n");
    
    do{
        type = chose_mode();
    }while(type != 1 && type != 2);
    
    switch (type){
        case 1:
            if(sign_in_register(type) ==1)  {
                return 1; // 登陆成功
            }
            else     
                start();
            break;
        case 2:
            sign_in_register(type);
            start();
            break;
        case 0:
            exit (1);
    }
}
int  chose_mode()//清空缓冲区问题
{
    int ret;
    setbuf(stdin, NULL);
    scanf("%d",&ret);
    return ret;
}
char getch()
{
    char c;
    system("stty -echo");  //不回显
    system("stty -icanon");//设置一次性读完操作，如使用getchar()读操作，不需要按enter
    c=getchar();
    system("stty icanon");//取消上面的设置
    system("stty echo");//回显
    return c;
}
void get_passd(char *passd)
{    
  
    char ch;
    int i = 0,j=15;

    while (j--) {
        ch  = getch();
        if(ch != '\n' &&  ch!=127 ) {
            printf("*");
            passd[i]=ch;
            i++;
        }
        else if(ch == '\n'&& i!=0) {
            passd[i]='\0';
            break;
        }
        else if(ch ==127 &&i>0) {
            passd[--i]='\0';
            printf("\b \b");
        }
    }
}
int  sign_in_register(int mode)//登录注册
{
    int ret;
    char  name[20];
    char  passd[20];
    char  passd_twice[20];
 
    memset(&name,0,sizeof(name)); 
    printf("账号 :");
   
    getchar();
    int n=20;
    int len =0;
    
    while(1){
        memset(name, 0 , sizeof(name));
        cin >> name;
        len = strlen(name);
        if(len <=2 ||len >=15){
            printf("您输入的有误，请重新输入\n");
            printf("账号 :");
        }    
        else 
	        break;
    }
    if (mode == 1){
        printf("密码 :");
        get_passd(passd);       
        
    }    
    if (mode == 2){
        do{
            memset(passd , 0, sizeof (passd));
            printf("\n密码 :");
            get_passd(passd_twice ) ;
            printf("\n密码 :");
            get_passd(passd);
            if (strcmp (passd_twice , passd ) == 0 )
                break;
            else 
                 printf("两次密码不一致请重新输入\n");
        }while(1);
    }
   // getchar();
    if( sql_mima(name , passd) ){
        
        cout <<"成功登录请指示"  <<endl;
        return  1;
    }
    else {

        cout << "登录失败" << endl;
        return 0;
    }
}
int sql_mima(char *name, char  *passd)
{
    char   sql[100];
    sprintf(sql,"SELECT * FROM account WHERE  name=%s AND passd =%s ",name,passd);
    //cout << sql <<endl;
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
            int column =  mysql_num_fields(res_ptr);
            int row    =  mysql_num_rows(res_ptr);  //查到几项  列数
            //输出结果的字段名
            for(int  i = 0; i< row ;i++){
                result_row = mysql_fetch_row(res_ptr);
               // arcs[atoidd(result_row[0])][atoi(result_row[1])] = atoi(result_row[2]); //atoi将字符串转化成数字
            }
            if(row == 1)
                return  1;
            else 
                return  0;
        }
        else 
            return  0;
    }    
}
/*
int main()
{
    int  num = start();

    cout  <<  "权限为" <<  num <<endl;
    


}
*/
