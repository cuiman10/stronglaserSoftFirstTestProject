#include <mysplineitem.h>
#include <QGraphicsItem>
#include <QPainter>
#include <qdebug.h>

//void mysplineItem:://构造函数
mysplineItem::mysplineItem(QVector <double> kv,
                           QVector <double> cpx,
                           QVector <double> cpy){
   // qDebug()<<"节点值";
    for(int i = 0;i<kv.length();i++){
        knotvalue.append(kv[i]);
//        qDebug()<<"kv"<<kv[i];
    }
   // qDebug()<<"节点值2"<<kv[1]<<" "<<kv[4];
    for(int i = 0;i<cpx.length();i++){
        controlpointx.append(cpx[i]);
        controlpointy.append(cpy[i]);
//        qDebug()<<"cpx"<<controlpointx[i];
    }
    ncontrol = controlpointx.length();
    nknot = knotvalue.length();
  //  qDebug()<<"构造函数test";
  //  qDebug()<<"nknot"<<nknot<<"kv length"<<kv.length();
}

//重写boundingRect矩形大小待测
QRectF mysplineItem::boundingRect() const{
    double maxx = controlpointx[0],maxy = controlpointy[0],
            minx = controlpointx[0],miny = controlpointy[0];
   // qDebug()<<"boundingRect test";
    for (int i = 0;i<ncontrol;i++){
        if(controlpointx[i]>maxx)
            maxx = controlpointx[i];
        if(controlpointy[i]>maxy)
            maxy = controlpointy[i];
        if(controlpointx[i]<minx)
            minx = controlpointx[i];
        if(controlpointy[i]<miny)
            miny = controlpointy[i];
    }
   // qDebug()<<"spline boundingRect测试2"<<minx<<"  "<<miny<<"  "<<maxx-minx<<"  "<<maxy-miny;
    return QRectF(minx,miny,maxx-minx,maxy-miny);
}

void mysplineItem::calknot(){
 //   qDebug()<<"calknot first test";
    knotvector.append(0);
    knotvector.append(0);
    knotvector.append(0);
    knotvector.append(0);
   // qDebug()<<"calknot second test"<<"  "<<nknot;
//    for(int i = 0;i<nknot;i++)
//        qDebug()<<knotvalue[i];
//    qDebug()<<"end";
    for(int i = 4;i<nknot;i++){

        knotvector.append(knotvalue[i]/knotvalue[nknot-2]);
       // qDebug()<<"calknot for fun test";

    }
//    for(int i = 0;i<nknot;i++)
//        qDebug()<<knotvector[i];
}


//求基函数
double mysplineItem::foundfun(int i,int m,double k){
    double n = 0;
    double a,b,c,d;
   // qDebug()<<"foundfun first test"<<" "<<m;
    if(m == 0){
        if(k>=knotvector[i]&&k<=knotvector[i+1])
            n = n+1;
        else n = n+0;
        return n;
    }
    n = n+basefun((k - knotvector[i])*foundfun(i,m-1,k),
                      knotvector[i+m]-knotvector[i])
                +basefun((knotvector[i+m+1]-k)*foundfun(i+1,m-1,k),
                knotvector[i+m+1]-knotvector[i+1]);
    return n;
}

//除法，分母为0整个除法结果为零
double mysplineItem::basefun(double parameter1,double parameter2){
    if(parameter2 == 0&&parameter1 == 0){
        parameter1 = 0;
        parameter2 = 1;
    }
    double result = parameter1/parameter2;
    return result;
}


//绘图函数
void mysplineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);
  //  qDebug()<<"paint first test";
    double px = 0,py = 0;//曲线上的某一个点
    double px2 = 0,py2 = 0;//分子
    double drawpx,drawpy,drawpx2,drawpy2;
    double denominator = 0;//分母

    for(int i = 0;i<ncontrol;i++){//计算初始点
        px2 = px2 + foundfun(i,3,0.1)*controlpointx[i];
        py2 = py2 + foundfun(i,3,0.1)*controlpointy[i];
        denominator = denominator + foundfun(i,3,0.1);
    }
//    qDebug()<<"paint second test";
//    qDebug()<<"denominator"<<denominator;
    drawpx2 = px2/denominator;
    drawpy2 = py2/denominator;
  //  qDebug()<<"diantest"<<drawpx2<<"  "<<drawpy2;
    //计算P(K)
    //qDebug()<<denominator;
    for(double k = 0.001;k<=1;k+=0.001 ){
        px = 0;
        py = 0;
        denominator = 0;
        for(int i = 0;i<ncontrol;i++){
            px = px + foundfun(i,3,k)*controlpointx[i];
            py = py + foundfun(i,3,k)*controlpointy[i];
            denominator = denominator + foundfun(i,3,k);
        }
        drawpx = px/denominator;
        drawpy = py/denominator;
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->drawLine(drawpx2,drawpy2,drawpx,drawpy);
     //   qDebug()<<"coordinate"<<drawpx2<<" "<<drawpy2<<" "<<drawpx<<" "<<drawpy;
        drawpx2 = drawpx;
        drawpy2 = drawpy;
    }
 //   qDebug()<<"paint third test   "<<denominator;

}
