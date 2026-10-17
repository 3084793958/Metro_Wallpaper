#include "metro_wallpaper.h"

#include <QHBoxLayout>
#include <QTime>
#include <QDateTime>
#include <QIcon>
#include <QImageReader>
#include <QCursor>
#include <QGuiApplication>
#include <QScreen>

#include <cmath>

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
    : QObject(parent)
{
    // 声明 0.0.3：新版宿主会据此调用 init_V_0_0_3 等扩展接口；
    // 旧版宿主只知道 0.0.1，按自己的判定只调用 init，因此 init 必须自给自足。
    Plugin_Version = P_Version{0, 0, 3};

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
        return QSize(0, 0);
    }
    QSize size = reader.size();
    if (!size.isValid())
    {
        QImage image = reader.read();
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

    // 尺寸确定后，按当前鼠标位置重新刷新一次视差
    if (m_mouseEffect)
    {
        mouseMoveEvent(QCursor::pos().x(), QCursor::pos().y());
    }
    else
    {
        wallpaper_item->move(m_basePoint);
    }
}

void Metro_Wallpaper::init(const Path_Info &info, P_Sender *send_position_sender)
{
    // 旧版 Easy_Desktop（0.0.1 接口时期）只会调用 init，
    // 之后不会再调任何扩展接口，因此这里必须用默认值搭出可独立运行的完整状态。
    (void) send_position_sender;

    // 0.0.1 时期确定存在的字段，可以直接读
    m_mouseEffect = info.mouse_effect;
    m_kMouseMoveWidth = info.k_mouse_move_width;
    m_kMouseMoveHeight = info.k_mouse_move_height;

    // mouse_control_type 是后续追加的字段（构造函数里带默认值即为佐证）。
    // 旧宿主传入的 Path_Info 可能没有这块内存，这里不读，
    // 先用默认值让插件跑起来，等 init_V_0_0_3 被调用时再修正。
    m_mouseControlType = Mouse_Control_Type::Follow_Desktop;

    m_basePoint = QPoint(0, 0);
    wallpaper_item->move(m_basePoint);

    if (m_mouseEffect)
    {
        // 若尺寸尚未确定，mouseMoveEvent 内部会直接返回
        mouseMoveEvent(QCursor::pos().x(), QCursor::pos().y());
    }
}

void Metro_Wallpaper::init_V_0_0_3(const Ext_Path_Info_V_0_0_1 &info,
                                   const QStringList &loaderNames,
                                   const P_Sender *seek_loader_sender,
                                   const P_Sender *recive_loader_sender)
{
    // 只有新版 Easy_Desktop 会走到这里，此时 Path_Info 一定是完整结构，
    // 可以安全读取扩展字段并修正 init 中的默认值。
    (void) loaderNames;
    (void) seek_loader_sender;
    (void) recive_loader_sender;

    m_mouseEffect = info.mouse_effect;
    m_kMouseMoveWidth = info.k_mouse_move_width;
    m_kMouseMoveHeight = info.k_mouse_move_height;

    Mouse_Control_Type ctrl = info.mouse_control_type;
    if (ctrl != Mouse_Control_Type::Follow_Desktop &&
        ctrl != Mouse_Control_Type::Follow_Wallpaper)
    {
        ctrl = Mouse_Control_Type::Follow_Desktop;
    }
    m_mouseControlType = ctrl;

    // 参数修正后立即重新应用一次
    if (m_mouseEffect)
    {
        mouseMoveEvent(QCursor::pos().x(), QCursor::pos().y());
    }
    else
    {
        wallpaper_item->move(m_basePoint);
    }
}

void Metro_Wallpaper::mouseMoveEvent(int mouse_x, int mouse_y)
{
    if (!m_mouseEffect)
    {
        wallpaper_item->move(m_basePoint);
        return;
    }

    const int wallpaper_w = wallpaper_item->width();
    const int wallpaper_h = wallpaper_item->height();
    if (wallpaper_w <= 0 || wallpaper_h <= 0)
    {
        return;
    }

    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen)
    {
        wallpaper_item->move(m_basePoint);
        return;
    }
    const QRect screen_rect = screen->geometry();
    const int screen_w = screen_rect.width();
    const int screen_h = screen_rect.height();
    if (screen_w <= 0 || screen_h <= 0)
    {
        wallpaper_item->move(m_basePoint);
        return;
    }

    int move_x = 0;
    int move_y = 0;

    switch (m_mouseControlType)
    {
    case Mouse_Control_Type::Follow_Desktop:
    {
        move_x = static_cast<int>(
            m_kMouseMoveWidth * (mouse_x - screen_w / 2) *
            (static_cast<double>(wallpaper_w) / screen_w));
        move_y = static_cast<int>(
            m_kMouseMoveHeight * (mouse_y - screen_h / 2) *
            (static_cast<double>(wallpaper_h) / screen_h));
        break;
    }
    case Mouse_Control_Type::Follow_Wallpaper:
    {
        const double nx = (static_cast<double>(mouse_x) * 2.0 / screen_w) - 1.0;
        const double ny = (static_cast<double>(mouse_y) * 2.0 / screen_h) - 1.0;

        move_x = static_cast<int>(
            wallpaper_w * std::abs(m_kMouseMoveWidth) / 2.0 * nx *
            (m_kMouseMoveWidth >= 0 ? 1 : -1));
        move_y = static_cast<int>(
            wallpaper_h * std::abs(m_kMouseMoveHeight) / 2.0 * ny *
            (m_kMouseMoveHeight >= 0 ? 1 : -1));
        break;
    }
    }

    wallpaper_item->move(m_basePoint + QPoint(move_x, move_y));
}

void Metro_Wallpaper::play()
{
    if (!timer->isActive())
    {
        timer->start(1000 - QTime::currentTime().msec());
    }
}

void Metro_Wallpaper::pause()
{
    timer->stop();
}
