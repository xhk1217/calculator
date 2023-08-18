//写了二十多个小时，不算上qt的学习时长，终于写完了！！！哈哈哈哈（花了快一周）\\小数未支持
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QString>
#include<qstack.h>
#include<QChar>
#include<QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    mistake=0;
    divi_zero=0;
    ui->setupUi(this);
    setWindowTitle("许恒锴的计算器");//在uI下面写！
    setFixedSize(381,360);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::tran_digital(QChar a)
{
    if (a == '|')
        return 1;
    else if (a == '&')
        return 2;
    else if (a == '+')
        return 3;
    else if (a == '-')
        return 3;
    else if (a == '*')
        return 4;
    else if (a == '/')
        return 4;
    else if (a == '!')
        return 5;

}

bool MainWindow::compare(QChar a, QChar b)
{
    if (a == '(')
        return 0;
    else if (b == '(')
        return 0;
    else if (b == ')')
        return 1;

    else if (b == '#')
        return 1;
    else if (a == '#')
        return 0;

    int _a = tran_digital(a);
    int _b = tran_digital(b);

    if (a == '!' && b == '!')
        return 0;

    else if (_a == _b)
        return 1;
    else if (_a > _b)
        return 1;
    else if(_a < _b)
        return 0;
}

bool MainWindow::is_digital(QChar a)
{
    if (a >= '0' && a <= '9')
        return 1;
    else
        return 0;
}
bool MainWindow::is_operation(QChar a)
{
    if (a == '(' || a == ')' || a == '#' || a == '+' || a == '-' || a == '*' || a == '/'||a=='!'||a=='&'||a=='|')
        return 1;
    else
        return 0;
}

void MainWindow::examine_mistake(QString a)//针对括号  比如2(+)3  //还有!前不能是数字，解决吧1！不会报错
{
//    int num1=0,num2=0;
//    for(int i=0;i<a.size();i++)
//    {
//        if(a[i]=='(')
//            num1++;
//        else if(a[i]==')')
//            num2++;
//    }
//    if(num1!=num2)
//        mistake=1;
    QStack<bool> m;

        for(int i=0;i<a.size();i++)
        {
        if(a[i]=='!'&&i!=0)
        {
            if(is_digital(a[i-1]))
            {
                mistake=1;
                return;
            }
        }
            if(a[i]=='(')
            {
            m.push(1);
                if(i!=a.size()-1)
                {
                    if(a[i+1]=='|'||a[i+1]=='&'||a[i+1]=='*'||a[i+1]=='/')//其他情况
                    {
                        mistake=1;
                        return;
                    }
                }
                if(i!=0)
                {
                    if(is_digital(a[i-1]))
                    {
                        mistake=1;
                        return;
                    }
                }
            }
            else if(a[i]==')')
            {
                if(m.size()==0)//解决括号不匹配，或者数目不相等的问题
                {
                mistake=1;
                return;
                }
                if(i!=a.size()-1)
                {
                    if(is_digital(a[i+1])||a[i+1]=='!')//其他情况,单目或者数字
                    {
                          mistake=1;
                          return;
                    }
                }
                if(i!=0)
                {
                    if(a[i-1]=='+'||a[i-1]=='-'||a[i-1]=='*'||a[i-1]=='/'||a[i-1]=='!'||a[i-1]=='|'||a[i-1]=='&')
                    {
                          mistake=1;
                          return;
                    }
                }

            m.pop();
            }
        }
        if(m.size()!=0)//数目不相等
        {
            mistake=1;
            return;
        }
}

int MainWindow::caculate()//ps.但是前后错误可能抵消，所以最好二元的左右，一元的右边数字或者（也检测一下），比较麻烦先不做了
{
    QStack<QChar> operation;
    QStack<int> digital;
    QStack<bool> addm;//记录加和减，是正负还是运算符//可以这样是因为栈是固定顺序进出（原理)
    if(cacu.size()==1)//也就是说只有一个#
    {
        return 0;
    }
    operation.push('#');
    //都可以，如果。。栈出问题也会报错
    for (int i = 0; 1/*i<cacu.size()*/; i++)//atten.之前写错了，写成了for (int i = 0; cacu.size(); i++)，但是也能返回，因为中间。。
        {
//        qDebug()<<mistake<<" "<<digital.size();
//        qDebug()<<operation.top()<<cacu[i];
        if (is_digital(cacu[i]))//qchar转换失败，准备改函数为qchar
            {
                if (i > 0)
                {
                    if (is_digital(cacu[i - 1]))
                    {
                        int m=digital.top();
                        digital.pop();
                        digital.push(m* 10 + cacu[i].digitValue());

                    }
                    if (!is_digital(cacu[i - 1]))
                        digital.push(cacu[i].digitValue());//因为cacu[i] - '0'是qchar-char
                }
                else//新加的
                {
                    digital.push(cacu[i].digitValue());
                }
            }

            else if (is_operation(cacu[i]))
            {

                if (operation.top() == '&' && cacu[i] == '&')
                    continue;
                if (operation.top() == '|' && cacu[i] == '|')
                    continue;

                //注意：不能放在这，因为只有入栈了才会运算，但是可能在这里没有入栈的元素的+-影响了，已经入栈的+-的运算符,比如+6-5
//                if(cacu[i]=='+'||cacu[i]=='-')
//                {
//                    //这里加减的条件是一样的
//                    if(i==0||cacu[i-1]=='|'||cacu[i-1]=='&'||cacu[i-1]=='(')//利用了||的特性，前一个i==0就不会去算下一个，i-1就不会<0，话说这个特新要怎么编写？？？
//                    {
//                        addm.push(0);//加减（二元）进1，正负（一元）进0
//                    }
//                    else
//                    {
//                        addm.push(1);
//                    }
//                }


                while (compare(operation.top(), cacu[i]))
                {
                    if (operation.top() == '#' && cacu[i] == '#')
                    {
//                        qDebug()<<digital.size();
                        if(digital.size()!=1)
                        {
                            mistake=1;
                            return 0;
                        }
                        int k=digital.top();
                        digital.pop();
                        return k;

                    }

                    if (operation.top() == '+')
                    {
                        operation.pop();
                            //不对，此时的i并不是刚刚获取+的i，刚刚获取+时就要标注每个+-或者不同的+-不同的符号（danuyuan
//                        if(i==0||cacu[i-1]=='|'||cacu[i-1]=='&'||cacu[i-1]=='(')//利用了||的特性，前一个i==0就不会去算下一个，i-1就不会<0，话说这个特新要怎么编写？？？
//                        {
//                            qDebug()<<2;
//                            if(digital.size()==0)
//                            {
//                                mistake=1;
//                                return 0;
//                            }
//                            int a = digital.top();
//                            digital.pop();
//                            digital.push(a);
//                        }
                        bool jud=addm.top();
                        addm.pop();
                        if(jud==0)
                        {
                            if(digital.size()==0)
                            {

                                mistake=1;
                                return 0;
                            }
                            int a = digital.top();
                            digital.pop();
                            digital.push(a);
                        }
                        else
                        {
                            if(digital.size()==0)
                            {
                                mistake=1;
                                return 0;
                            }
                        int a = digital.top();
                        digital.pop();
                        if(digital.size()==0)
                        {
                                mistake=1;
                                return 0;
                        }
                        int b = digital.top();
                        digital.pop();
                        digital.push(a + b);
                        }
                    }
                    else if (operation.top() == '-')
                    {
                        operation.pop();
//                        if(i==0||cacu[i-1]=='|'||cacu[i-1]=='&'||cacu[i-1]=='(')//利用了||的特性，前一个i==0就不会去算下一个，i-1就不会<0，话说这个特新要怎么编写？？？
//                        {
//                        if(digital.size()==0)
//                        {

//                                mistake=1;
//                                return 0;
//                        }
//                        int a = digital.top();
//                        digital.pop();
//                        digital.push(-a);
//                        }
                        bool jud=addm.top();
                        addm.pop();
                        if(jud==0)
                        {
                                               if(digital.size()==0)
                                                {

                                                        mistake=1;
                                                        return 0;
                                                }
                                                int a = digital.top();
                                                digital.pop();
                                                digital.push(-a);
                        }
                        else
                        {
                        if(digital.size()==0)
                        {
                                mistake=1;
                                return 0;
                        }
                        int a = digital.top();
                        digital.pop();
                        if(digital.size()==0)
                        {
                                mistake=1;
                                return 0;
                        }
                        int b = digital.top();
                        digital.pop();
                        digital.push(b - a);
                        }
                    }
                    else if (operation.top() == '*')
                    {
                        operation.pop();
                        if(digital.size()==0)
                        {
                        mistake=1;
                        return 0;
                        }
                        int a = digital.top();
                        digital.pop();
                        if(digital.size()==0)
                        {
                        mistake=1;
                        return 0;
                        }
                        int b = digital.top();
                        digital.pop();
                        digital.push(a * b);
                    }
                    else if (operation.top() == '/')
                    {
                        operation.pop();
                        if(digital.size()==0)
                        {
                        mistake=1;
                        return 0;
                        }
                        int a = digital.top();
                        digital.pop();
                        if(digital.size()==0)
                        {
                        mistake=1;
                        return 0;
                        }
                        int b = digital.top();
                        digital.pop();
                        if(a==0)
                        {
                            divi_zero=1;
                            mistake=1;
                            return 0;
                        }
                        else
                        digital.push(1.0 / a * b);
                    }

                    //1
                    else if (operation.top() == '!')
                    {
                        operation.pop();
                        if(digital.size()==0)
                        {
                        mistake=1;
                        return 0;
                        }
                        int a = digital.top();
                        digital.pop();
                        if (a)
                        {
                            digital.push(0);
                        }
                        else
                        {
                            digital.push(1);
                        }
                    }

                    else if (operation.top() == '&')
                    {
                        operation.pop();
                        if(digital.size()==0)
                        {
                            mistake=1;
                            return 0;
                        }
                        int a = digital.top();
                        digital.pop();
                        if(digital.size()==0)
                        {
                            mistake=1;
                            return 0;
                        }
                        int b = digital.top();
                        digital.pop();
                        digital.push(a && b);
                    }
                    else if (operation.top() == '|')
                    {
                        operation.pop();
                        if(digital.size()==0)
                        {
                            mistake=1;
                            return 0;
                        }
                        int a = digital.top();
                        digital.pop();
                        if(digital.size()==0)
                        {
                            mistake=1;
                            return 0;
                        }
                        int b = digital.top();
                        digital.pop();
                        digital.push(a || b);
                    }


                }
                if (!compare(operation.top(), cacu[i]))
                {
//                    qDebug()<<"之心";
                    operation.push(cacu[i]);
                }

                //在这里能入栈应该入栈了
                if(cacu[i]=='+'||cacu[i]=='-')
                {
                    //这里加减的条件是一样的
                    if(i==0||cacu[i-1]=='|'||cacu[i-1]=='&'||cacu[i-1]=='(')//利用了||的特性，前一个i==0就不会去算下一个，i-1就不会<0，话说这个特新要怎么编写？？？
                    {
                        addm.push(0);//加减（二元）进1，正负（一元）进0
                    }
                    else
                    {
                        addm.push(1);
                    }
                }

                if (operation.top() == ')')
                {
                    operation.pop();
                    operation.pop();
                }
            }
        }
    mistake=1;//应该用不到，蛮加的
    return 0;
}








void MainWindow::on_pushButton_clicked()
{
    cacu=cacu+"1";
    ui->textEdit->setText(cacu);
}



void MainWindow::on_pushButton_2_clicked()
{
    cacu=cacu+"2";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_3_clicked()
{
    cacu=cacu+"3";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_4_clicked()
{
    cacu=cacu+"4";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_5_clicked()
{
    cacu=cacu+"5";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_6_clicked()
{
    cacu=cacu+"6";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_7_clicked()
{
    cacu=cacu+"7";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_8_clicked()
{
    cacu=cacu+"8";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_9_clicked()
{
    cacu=cacu+"9";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_10_clicked()
{
    cacu=cacu+"0";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_19_clicked()
{
    cacu=cacu+"!";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_11_clicked()
{
    cacu=cacu+"&&";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_12_clicked()
{
    cacu=cacu+"||";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_13_clicked()
{
    cacu=cacu+"+";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_14_clicked()
{
    cacu=cacu+"-";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_15_clicked()
{
    cacu=cacu+"*";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_16_clicked()
{
    cacu=cacu+"/";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_17_clicked()
{
    cacu.clear();
    ui->textEdit->setText(cacu);
    ui->textEdit_2->setText("");
}






void MainWindow::on_pushButton_18_clicked()//计算器怎么知道无法计算？？
{
    QString temp=cacu;
    examine_mistake(cacu);//此时没有#
//    qDebug()<<mistake;
    if(mistake==1)
    {  cacu.clear(); ui->textEdit->setText("输入的表达式错误");ui->textEdit_2->setText(temp);mistake=0;return;}
    cacu=cacu+"#";
    int i=caculate();
//    qDebug()<<mistake;
    cacu.clear();
//    qDebug()<<i;测试
    ui->textEdit_2->setText(temp);
    if(mistake==0)
    ui->textEdit->setText(QString::number(i));
    else if(mistake==1&&divi_zero==1)
    {
    ui->textEdit->setText("输入的表达式错误（出现了/0）");
    mistake=0;
    }
    else
    {
    ui->textEdit->setText("输入的表达式错误");
    mistake=0;
    }
}


void MainWindow::on_pushButton_20_clicked()
{
    cacu=cacu+"(";
    ui->textEdit->setText(cacu);
}


void MainWindow::on_pushButton_21_clicked()
{
    cacu=cacu+")";
    ui->textEdit->setText(cacu);
}

