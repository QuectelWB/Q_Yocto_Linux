/*******
$ sudo apt update
$ sudo apt install mpv
$ sudo apt install libmpv-dev
 *
 *
 *	https://github.com/mpv-player/mpv-examples
 *
 * */

#include <QApplication>
#include <QWidget>
#include <QtDebug>

#include <mpv/client.h>

static inline void checkError(int status)
{
    if (status < 0) {
        qWarning() << QString("mpv error: %1").arg(mpv_error_string(status));
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    /*
     * QApplication 设置了 setlocale(LC_ALL, "")，
     * 但 mpv 需要将 LC_NUMERIC 设置为 "C"，所以得改回来。
    */
    std::setlocale(LC_NUMERIC, "C");

    // 创建一个 Qt 界面
    QWidget widget;
    widget.setMinimumSize(640, 480);

    // 创建 mpv 实例
    mpv_handle *mpv = mpv_create();
    if (mpv == nullptr) {
        qWarning() << "failed creating context";
        return 1;
    }

    // 设置播放句柄
    WId wid = widget.winId();
    mpv_set_property(mpv, "wid", MPV_FORMAT_INT64, &wid);

    /*
     * 启用默认的按键绑定，这样就可以与播放器交互了
     * 例如：{ 和 } 倍速播放、P/SPACE 键暂停/恢复
     * 通常使用 mpv 作为后台的播放器，会实现自己的按键绑定
    */
    mpv_set_option_string(mpv, "input-default-bindings", "yes");

    // 启用 X11 窗口的键盘输入
    mpv_set_option_string(mpv, "input-vo-keyboard", "yes");

    // 设置音量
    double volume = 60.0;
    checkError(mpv_set_property(mpv, "volume", MPV_FORMAT_DOUBLE, &volume));

    // 初始化 mpv 实例
    checkError(mpv_initialize(mpv));

    // 启用日志（级别为 Info 或更高）
    checkError(mpv_request_log_messages(mpv, "info"));

    // 播放文件
    QString fileName = "/home/waleon/video/big_buck_bunny.mkv";
    QByteArray file = fileName.toUtf8();
    const char *args[] = {"loadfile", file.constData(), Q_NULLPTR};
    mpv_command_async(mpv, 0, args);

    // 显示界面
    widget.show();

    int ret = app.exec();

    // 释放 mpv 实例
    mpv_terminate_destroy(mpv);

    return ret;
}
