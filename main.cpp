#include <QCoreApplication>
#include <QImage>
#include <QPixmap>
#include <QFile>
#include <QDebug>
#include <QtMath>
#include <QTextStream>
#include <iostream>

using namespace std;

QRgb mix(QRgb r1, QRgb r2){
    return qRgb((qRed(r1)+qRed(r2))/2,
                 (qGreen(r1)+qGreen(r2))/2,
                 (qBlue(r1)+qBlue(r2))/2);
}

/*=============================================================================
    ENCODE
=============================================================================*/
QImage * encode(QImage * img){
    int w = img->width()/2;
    int h = img->height();

    QImage * n = new QImage(w, h/2, QImage::Format_RGB888);

    int i, j;
    QRgb pixel;

    for(i=0; i<h; ++i)
    {
        for(j=0; j<w; ++j){
            if((j*2)+1>w)
                pixel = mix( img->pixel(j*2,i),img->pixel((j*2)+1,i) );
            else
                pixel = img->pixel(j*2,i);
            img->setPixel(j,i,pixel);
        }
    }
    h/=2;
    for(i=0; i<w; ++i)
    {
        for(j=0; j<h; ++j){
            if((j*2)+1>h)
                pixel = mix( img->pixel(i,j*2),img->pixel(i,(j*2)+1) );
            else
                pixel = img->pixel(i,j*2);
            n->setPixel(i,j,pixel);
        }
    }

    return n;
}

/*=============================================================================
    MAIN
=============================================================================*/
int main()
{
    QImage * img;
    QTextStream qstdin(stdin);
    qDebug() << "Enter the file path:";
    QString fileName = qstdin.readLine().trimmed();

    if( fileName.isNull() || fileName.isEmpty() ){
        qDebug() << "Invalid file name (absolute path)\n";
        return EXIT_FAILURE;
    }

    img = new QImage(fileName);

    if( img->isNull() ){
        qDebug() << "Imagem don't exist\n";
        return EXIT_FAILURE;
    }

    //  Converte para 3bytes por pixel. ter certeza q esta no padrao
    *img = img->convertToFormat(QImage::Format_RGB888);

    qDebug() << "======================================================";
    qDebug() << "Before: " << img->byteCount()/1024 << "kb" << "; " << img->size();

    //  Escala para 1/2 e salva para comparacao
    img->scaled(img->width()/2,img->height()/2).save(fileName + "_scaled.bmp");

    //  Codifica e salva
    img = encode(img);
    img->save(fileName + "_encoded.bmp");


    qDebug() << "After:  " << img->byteCount()/1024 << "kb" << "; " << img->size();
    qDebug() << "======================================================";

    return EXIT_SUCCESS;
}
