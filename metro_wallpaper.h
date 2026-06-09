#ifndef METRO_WALLPAPER_H
#define METRO_WALLPAPER_H

#include "interfaces/wallpaper/wallpaper_plugin.h"
#include <QWidget>
#include <QTimer>
#include <QLabel>

class Metro_Wallpaper : public QObject, public Ext_Wallpaper_PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.Easy.Desktop.Ext_Wallpaper_PluginInterface" FILE "Metro_Wallpaper.json")
    Q_INTERFACES(Ext_Wallpaper_PluginInterface)
public:
    explicit Metro_Wallpaper(QObject *parent = nullptr);
    virtual ~Metro_Wallpaper() override;
    virtual void RemovePlugin() override;
    virtual QWidget *wallpaperItem() override;
    virtual void sizeChange(QSize size) override;
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
};

#endif // METRO_WALLPAPER_H
