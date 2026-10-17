#ifndef METRO_WALLPAPER_H
#define METRO_WALLPAPER_H

#include "interfaces/wallpaper/wallpaper_plugin.h"
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QPoint>

class Metro_Wallpaper : public QObject, public Ext_Wallpaper_PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.Easy.Desktop.Ext_Wallpaper_PluginInterface" FILE "Metro_Wallpaper.json")
    Q_INTERFACES(Ext_Wallpaper_PluginInterface)
public:
    explicit Metro_Wallpaper(QObject *parent = nullptr);
    virtual ~Metro_Wallpaper() override;

    // 0.0.1 接口：旧版 Easy_Desktop 只会调用这些
    virtual void RemovePlugin() override;
    virtual QWidget *wallpaperItem() override;
    void init(const Path_Info &info, P_Sender *send_position_sender) override;
    void mouseMoveEvent(int mouse_x, int mouse_y) override;
    void play() override;
    void pause() override;
    void sizeChange(QSize size) override;

    // 0.0.3 接口：新版宿主才会调用，用于修正 init 中不可靠的扩展字段
    void init_V_0_0_3(const Ext_Path_Info_V_0_0_1 &info,
                      const QStringList &loaderNames,
                      const P_Sender *seek_loader_sender,
                      const P_Sender *recive_loader_sender) override;

private:
    QWidget *wallpaper_item = new QWidget;
    QLabel *line_Map = new QLabel(wallpaper_item);
    QLabel *time_h1 = new QLabel(wallpaper_item);
    QLabel *time_h2 = new QLabel(wallpaper_item);
    QLabel *time_m1 = new QLabel(wallpaper_item);
    QLabel *time_m2 = new QLabel(wallpaper_item);
    QLabel *time_s1 = new QLabel(wallpaper_item);
    QLabel *time_s2 = new QLabel(wallpaper_item);
    QTimer *timer = new QTimer(this);

    bool m_mouseEffect = false;
    qreal m_kMouseMoveWidth = 0.0;
    qreal m_kMouseMoveHeight = 0.0;
    Mouse_Control_Type m_mouseControlType = Mouse_Control_Type::Follow_Desktop;
    QPoint m_basePoint = QPoint(0, 0);
};

#endif // METRO_WALLPAPER_H
