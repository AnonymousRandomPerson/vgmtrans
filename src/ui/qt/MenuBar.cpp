/*
 * VGMTrans (c) 2002-2019
 * Licensed under the zlib license,
 * refer to the included LICENSE.txt file
 */

#include "MenuBar.h"

#include <QActionGroup>
#include <QDockWidget>
#include "MusicPlayer.h"

MenuBar::MenuBar(QWidget *parent, const QList<QDockWidget *>& dockWidgets) : QMenuBar(parent) {
  appendFileMenu();
  appendOptionsMenu(dockWidgets);
  appendInfoMenu();
}

void MenuBar::appendFileMenu() {
  QMenu *file_dropdown = addMenu("File");
  menu_open_file = file_dropdown->addAction("Open");
  menu_open_file->setShortcut(QKeySequence(QStringLiteral("Ctrl+O")));
  connect(menu_open_file, &QAction::triggered, this, &MenuBar::openFile);

  file_dropdown->addSeparator();

  menu_app_exit = file_dropdown->addAction("Exit");
  menu_app_exit->setShortcut(QKeySequence(QStringLiteral("Alt+F4")));
  connect(menu_app_exit, &QAction::triggered, this, &MenuBar::exit);
}

void MenuBar::appendOptionsMenu(const QList<QDockWidget *>& dockWidgets) {
  QMenu *options_dropdown = addMenu("Options");
  auto audio_backend = options_dropdown->addMenu("Player audio driver");
  menu_drivers = new QActionGroup(this);

  for (auto &driver : MusicPlayer::the().getAvailableDrivers()) {
    auto driveropt = audio_backend->addAction(QString(driver));
    menu_drivers->addAction(driveropt);

    driveropt->setCheckable(true);
    if (MusicPlayer::the().checkSetting("audio.driver", driver)) {
      driveropt->setChecked(true);
      auto font = driveropt->font();
      font.setBold(true);
      driveropt->setFont(font);
    }
  }

  connect(menu_drivers, &QActionGroup::triggered, [](QAction *driver) {
    MusicPlayer::the().setAudioDriver(driver->text().toStdString().c_str());
  });

  options_dropdown->addSeparator();

  for(auto& widget : dockWidgets) {
    options_dropdown->addAction(widget->toggleViewAction());
  }
}

void MenuBar::appendInfoMenu() {
  QMenu *info_dropdown = addMenu("Help");
  menu_about_dlg = info_dropdown->addAction("About VGMTrans");
  connect(menu_about_dlg, &QAction::triggered, this, &MenuBar::showAbout);
}
