#pragma once

#include "components/config.hpp"
#include "config.hpp"
#include "modules/meta/static_module.hpp"
#include "x11/randr.hpp"

POLYBAR_NS

class connection;

namespace modules {
  /**
   * Backlight module built using the RandR X extension.
   *
   * This is built as a replacement for the old backlight
   * module that was set up using with inotify watches listening
   * for changes to the raw file handle.
   *
   * This module is alot faster, it's more responsive and it will
   * be dormant until new values are reported. Inotify watches
   * are a bit random when it comes to proc-/sysfs.
   *
   * TODO: Implement backlight configuring using scroll events
   */
  class xbacklight_module : public static_module<xbacklight_module>, public xpp::event::sink<evt::randr_notify> {
   public:
    xbacklight_module(const bar_settings& bar, const logger& logger, const config& config, string name);

    void setup();
    void teardown();
    void handle(const evt::randr_notify& evt);
    void update();
    string get_output();
    bool build(builder* builder, const string& tag) const;
    bool handle_event(string cmd);
    bool receive_events() const {
      return true;
    }

   private:
    static constexpr auto TAG_LABEL = "<label>";
    static constexpr auto TAG_BAR = "<bar>";
    static constexpr auto TAG_RAMP = "<ramp>";

    static constexpr auto EVENT_SCROLLUP = "xbacklight+";
    static constexpr auto EVENT_SCROLLDOWN = "xbacklight-";

    connection& m_connection;
    monitor_t m_output;
    xcb_window_t m_proxy;
    event_timer m_randrnotify{};

    ramp_t m_ramp;
    label_t m_label;
    progressbar_t m_progressbar;

    bool m_scroll = true;
    int m_percentage = 0;
  };
}

POLYBAR_NS_END
