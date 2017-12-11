#include "baresipcore.h"
#include "baresipvidisp.h"

#include <re.h>
#include <baresip.h>

void BaresipCore::run()
{
    //BaresipVidisp::m_vidisp->moveToThread(this);
    int ret;
    const char* conf_path = "/etc/ewindow";
    libre_init();
    conf_path_set(conf_path);
    conf_configure();

    //log_enable_debug(true);
    log_enable_info(true);

    bool enable_tls = false;
    bool prefer_ipv6 = true;

    ret = baresip_init(conf_config(), prefer_ipv6);
    ret |= ua_init("baresip title", true, true, enable_tls, prefer_ipv6);
    if (ret) {
        puts("Error in ua_init(), exiting...");
        exit(ret);
    }

    conf_modules();

    // TODO: overwrite some basic conf/settings, like video_display?

    struct vidisp* st;
    ret = vidisp_register(&st, baresip_vidispl(), "qtupload",
                    BaresipVidisp::alloc, 0, BaresipVidisp::display, 0);
    qInfo() << "vidisp_register " << ret;

//        tmr_init(&m_timer);
//        tmr_start(&m_timer, 100, re_callback, this);

    re_main(0);
}
