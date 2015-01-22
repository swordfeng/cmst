/**************************** controlbox.h *****************************

Code to manage the primary user interface to include the QDialog the
user interfaces with and the system tray icon.  

Copyright (C) 2013-2015
by: Andrew J. Bibb
License: MIT 

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"),to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions: 

The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.
***********************************************************************/ 

# ifndef CONTROLBOX_H
# define CONTROLBOX_H

# include <QtDBus/QtDBus>
# include <QtDBus/QDBusInterface>
# include <QtDBus/QDBusMessage>
# include <QtDBus/QDBusObjectPath>
# include <QDialog>
# include <QString>
# include <QMap>
# include <QVariant>
# include <QSystemTrayIcon>
# include <QAction>
# include <QActionGroup>
# include <QCommandLineParser>
# include <QMenu>
# include <QSettings>
# include <QLocalServer>
# include <QFrame>
# include <QProgressBar>
# include <QColor>
# include <QToolButton>

# include "ui_controlbox.h"
# include "./code/agent/agent.h"
# include "./code/counter/counter.h"
# include "./code/notify/notify.h"


//  Two of the connman.Manager query functions will return an array of structures.
//  This struct provides a receiving element we can use to collect the return data.
struct arrayElement
{
  QDBusObjectPath objpath;
  QMap<QString,QVariant> objmap;
};

//
// custom QFrame containing a QToolButton that will emit a button id
class idButton : public QFrame
{
  Q_OBJECT
  
  public:
    idButton (QWidget*, const QDBusObjectPath&);
    inline void setText (const QString& text) {button->setText(text);}
    inline void setIcon (const QPixmap& pixmap) {button->setIcon(pixmap);}
    inline void setChecked (bool checked) {button->setChecked(checked);}    
   
  signals:
    void clickedID(QString, bool);
  
  private:
    QToolButton* button;
    QDBusObjectPath obj_id;
    
  private slots:
    void buttonClicked(bool);
};

//
// custom QFrame containing a QProgressBar
class SignalBar : public QFrame
{
  Q_OBJECT
  
  public:
    SignalBar (QWidget*);
    inline void setBarValue (const int& val) {bar->setValue(val);}
    
  private:
    QProgressBar* bar;      
};

//
//  The main program class based on a QDialog
class ControlBox : public QDialog
{
  Q_OBJECT

  public:
    ControlBox(const QCommandLineParser&, QWidget* parent = 0);

  public slots:  
    void aboutCMST();
    void aboutIconSet();
    void showLicense();
    void showChangeLog();
    
  protected:
    void closeEvent(QCloseEvent*);
    bool eventFilter(QObject*, QEvent*);   
    
  private:
  // members 
    Ui::ControlBox ui;
    quint8 q8_errors;
    QMap<QString,QVariant>  properties_map;
    QList<arrayElement>   services_list;
    QList<arrayElement>   technologies_list;
    QList<arrayElement>   wifi_list;
    QList<arrayElement>   peer_list;
    ConnmanAgent* agent;
    ConnmanCounter* counter;  
    NotifyClient* notifyclient; 
    short wifi_interval;    
    quint32 counter_accuracy; 
    quint32 counter_period;       
    QDBusInterface* iface_manager;
    QSystemTrayIcon*  trayicon;
    QMenu* trayiconmenu; 
    QActionGroup* minMaxGroup;
    QActionGroup* moveGroup;
    QAction* minimizeAction;
    QAction* maximizeAction;
    QAction* exitAction;
    bool b_useicontheme;
    bool b_usexfce;
    bool b_usemate;
    QMenu* mvsrv_menu;
    QSettings* settings;
    QString onlineobjectpath;
    QLocalServer* socketserver;
    QColor trayiconbackground;
  
  // functions
    int managerRescan(const int& srv = 0);
    void assemblePage1();
    void assemblePage2();
    void assemblePage3();
    void assemblePage4();
    void assembleTrayIcon();
    void createSystemTrayIcon(bool);
    void sendNotifications();
    bool getProperties();
    bool getTechnologies();   
    bool getServices();
    bool getArray(QList<arrayElement>&, const QDBusMessage&);
    bool getMap(QMap<QString,QVariant>&, const QDBusMessage&); 
    static bool extractMapData(QMap<QString,QVariant>&,const QVariant&);
    void logErrors(const quint8&);
    QString readResourceText(const char*);
    void clearCounters();
    static QVariant translateVariant(const QVariant&);

  private slots:
    void updateDisplayWidgets();
    void moveService(QAction*);
    void moveButtonPressed(QAction*);
    void enableMoveButtons(int,int);
    void counterUpdated(const QDBusObjectPath&, const QString&, const QString&);
    void connectPressed();
    void disconnectPressed();
    void removePressed();
    void dbsPropertyChanged(QString,QDBusVariant);
    void dbsServicesChanged(QMap<QString, QVariant>, QList<QDBusObjectPath>, QDBusMessage);
    void dbsPeersChanged(QMap<QString, QVariant>, QList<QDBusObjectPath>, QDBusMessage);
    void dbsServicePropertyChanged(QString, QDBusVariant, QDBusMessage);
    void dbsTechnologyPropertyChanged(QString, QDBusVariant, QDBusMessage);
    void dbsTechnologyAdded(QDBusObjectPath, QVariantMap);
    void dbsTechnologyRemoved(QDBusObjectPath);
    void scanWiFi();
    void toggleOfflineMode(bool);
    void toggleTrayIcon(bool);
    void togglePowered(QString, bool);
    void minMaxWindow(QAction* = 0);
    void getServiceDetails(int);
    void showWhatsThis();
    inline void startSystemTrayMinimized() {createSystemTrayIcon(true);}
    inline void startSystemTrayNormal() {createSystemTrayIcon(false);}
    inline void trayNotifications(bool checked) {if (checked) ui.checkBox_notifydaemon->setChecked(false);}
    inline void daemonNotifications(bool checked) {if (checked) ui.checkBox_systemtraynotifications->setChecked(false);}
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void enableRunOnStartup(bool enabled);
    void writeSettings();
    void readSettings();
    void connectNotifyClient();
    void configureService();
    void provisionService();
    void socketConnectionDetected();
    void cleanUp();
};

#endif

