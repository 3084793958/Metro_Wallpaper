#include "metro_wallpaper.h"
#include <QHBoxLayout>
#include <QTime>
#include <QIcon>
#include <QImageReader>
static QList<QString> Metro_Pic_List =
{
    ":/GZ_METRO/0.png",
    ":/GZ_METRO/1.png",
    ":/GZ_METRO/2.png",
    ":/GZ_METRO/3.png",
    ":/GZ_METRO/4.png",
    ":/GZ_METRO/5.png",
    ":/GZ_METRO/6.png",
    ":/GZ_METRO/7.png",
    ":/GZ_METRO/8.png",
    ":/GZ_METRO/9.png"
};
Metro_Wallpaper::Metro_Wallpaper(QObject *parent)
    :QObject(parent)
{
    time_h1->resize(300, 300);
    time_h2->resize(300, 300);
    time_m1->resize(300, 300);
    time_m2->resize(300, 300);
    time_s1->resize(300, 300);
    time_s2->resize(300, 300);
    QHBoxLayout *layout = new QHBoxLayout(wallpaper_item);
    layout->setSpacing(10);
    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(time_h1);
    layout->addWidget(time_h2);
    layout->addSpacing(40);
    layout->addWidget(time_m1);
    layout->addWidget(time_m2);
    layout->addSpacing(40);
    layout->addWidget(time_s1);
    layout->addWidget(time_s2);
    wallpaper_item->setLayout(layout);
    connect(timer, &QTimer::timeout, this, [=]
    {
        timer->setInterval(1000);
        QString text = QDateTime::currentDateTime().toString("hh:mm:ss");
        time_h1->setPixmap(QIcon(Metro_Pic_List[QString(text.at(0)).toInt()]).pixmap(wallpaper_item->width() / 6 - 30, wallpaper_item->width() / 6 - 30));
        time_h2->setPixmap(QIcon(Metro_Pic_List[QString(text.at(1)).toInt()]).pixmap(wallpaper_item->width() / 6 - 30, wallpaper_item->width() / 6 - 30));
        time_m1->setPixmap(QIcon(Metro_Pic_List[QString(text.at(3)).toInt()]).pixmap(wallpaper_item->width() / 6 - 30, wallpaper_item->width() / 6 - 30));
        time_m2->setPixmap(QIcon(Metro_Pic_List[QString(text.at(4)).toInt()]).pixmap(wallpaper_item->width() / 6 - 30, wallpaper_item->width() / 6 - 30));
        time_s1->setPixmap(QIcon(Metro_Pic_List[QString(text.at(6)).toInt()]).pixmap(wallpaper_item->width() / 6 - 30, wallpaper_item->width() / 6 - 30));
        time_s2->setPixmap(QIcon(Metro_Pic_List[QString(text.at(7)).toInt()]).pixmap(wallpaper_item->width() / 6 - 30, wallpaper_item->width() / 6 - 30));
    });
    timer->start(1000 - QTime::currentTime().msec());
}
Metro_Wallpaper::~Metro_Wallpaper()
{
    RemovePlugin();
}
void Metro_Wallpaper::RemovePlugin()
{
    timer->stop();
    wallpaper_item->deleteLater();
}
QWidget *Metro_Wallpaper::wallpaperItem()
{
    return wallpaper_item;
}
static QSize get_Image_Size(QString path)
{
    QImageReader reader(path);
    if (!reader.canRead())
    {
        return QSize(0,0);
    }
    QSize size = reader.size();
    if (!size.isValid())
    {
        QImage image =reader.read();
        if (!image.isNull())
        {
            size = image.size();
        }
    }
    return size;
}
void Metro_Wallpaper::sizeChange(QSize size)
{
    wallpaper_item->resize(size);
    QSize image_size = get_Image_Size(":/GZ_METRO/GMM.png");
    float R_width = static_cast<float>(size.width()) / image_size.width();
    float R_height = static_cast<float>(size.height()) / image_size.height();
    if (R_width > R_height)
    {
        int final_height = static_cast<int>(image_size.height() * (static_cast<float>(size.width()) / image_size.width()));
        image_size = QSize(size.width(), final_height);
    }
    else
    {
        int final_width = static_cast<int>(image_size.width() * (static_cast<float>(size.height()) / image_size.height()));
        image_size = QSize(final_width, size.height());
    }
    line_Map->setPixmap(QIcon(":/GZ_METRO/GMM.png").pixmap(image_size));
    line_Map->resize(size);
}
