/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionv1_0;
    QAction *action;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QTableWidget *tableWidget;
    QGridLayout *gridLayout;
    QPushButton *pushButton_4;
    QLabel *label;
    QPushButton *pushButton_3;
    QProgressBar *progressBar;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_4;
    QMenuBar *menuBar;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(700, 500);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(700, 500));
        MainWindow->setMaximumSize(QSize(700, 500));
        actionv1_0 = new QAction(MainWindow);
        actionv1_0->setObjectName(QStringLiteral("actionv1_0"));
        actionv1_0->setEnabled(false);
        action = new QAction(MainWindow);
        action->setObjectName(QStringLiteral("action"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setEnabled(true);

        gridLayout_2->addWidget(pushButton, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(135, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 0, 1, 1);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFrameShape(QFrame::WinPanel);
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_2, 3, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(135, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 2, 1, 1);

        tableWidget = new QTableWidget(centralWidget);
        if (tableWidget->columnCount() < 3)
            tableWidget->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        if (tableWidget->rowCount() < 6)
            tableWidget->setRowCount(6);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(2, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(3, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(4, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(5, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableWidget->setItem(0, 0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableWidget->setItem(0, 1, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableWidget->setItem(0, 2, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableWidget->setItem(1, 0, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableWidget->setItem(1, 1, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableWidget->setItem(1, 2, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableWidget->setItem(2, 0, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableWidget->setItem(2, 1, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableWidget->setItem(2, 2, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableWidget->setItem(3, 0, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableWidget->setItem(3, 1, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        tableWidget->setItem(3, 2, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        tableWidget->setItem(4, 0, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        tableWidget->setItem(4, 1, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        tableWidget->setItem(4, 2, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        tableWidget->setItem(5, 0, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        tableWidget->setItem(5, 1, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        tableWidget->setItem(5, 2, __qtablewidgetitem26);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy1);
        QFont font;
        font.setPointSize(8);
        font.setStyleStrategy(QFont::PreferAntialias);
        tableWidget->setFont(font);
        tableWidget->setFocusPolicy(Qt::StrongFocus);
        tableWidget->setMidLineWidth(0);
        tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        tableWidget->setAutoScroll(false);
        tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
        tableWidget->setRowCount(6);
        tableWidget->setColumnCount(3);
        tableWidget->horizontalHeader()->setVisible(true);
        tableWidget->horizontalHeader()->setDefaultSectionSize(180);
        tableWidget->horizontalHeader()->setMinimumSectionSize(10);
        tableWidget->horizontalHeader()->setStretchLastSection(false);
        tableWidget->verticalHeader()->setVisible(false);
        tableWidget->verticalHeader()->setMinimumSectionSize(50);
        tableWidget->verticalHeader()->setStretchLastSection(true);

        gridLayout_2->addWidget(tableWidget, 1, 0, 1, 3);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton_4 = new QPushButton(centralWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        gridLayout->addWidget(pushButton_4, 0, 0, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 1, 1, 1);

        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        gridLayout->addWidget(pushButton_3, 1, 0, 1, 1);

        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(24);

        gridLayout->addWidget(progressBar, 1, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 4, 0, 1, 3);

        horizontalSpacer_3 = new QSpacerItem(135, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 2, 0, 1, 1);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout_2->addWidget(pushButton_2, 2, 1, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(135, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 2, 2, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 700, 26));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menu->addAction(actionv1_0);
        menu->addAction(action);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\320\232\320\276\320\274\320\277\320\276\320\275\320\265\320\275\321\202\321\213 \320\264\320\273\321\217 \320\267\320\260\320\272\321\203\320\277\320\276\320\272 v.3.0.0.0 \320\276\321\202 04.12.2017", Q_NULLPTR));
        actionv1_0->setText(QApplication::translate("MainWindow", "v3.0.0.0", Q_NULLPTR));
        action->setText(QApplication::translate("MainWindow", "\320\236\320\277\320\270\321\201\320\260\320\275\320\270\320\265", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MainWindow", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214 BOM \321\204\320\260\320\271\320\273\321\213", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "d:/projects/gpAll/\321\201\320\272\320\273\320\260\320\264  03.10.17.xlsx", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "BOM \321\204\320\260\320\271\320\273\321\213", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "\320\232\320\276\320\273-\320\262\320\276", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "%", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "\320\235\320\276\320\262\320\260\321\217 \321\201\321\202\321\200\320\276\320\272\320\260", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "\320\235\320\276\320\262\320\260\321\217 \321\201\321\202\321\200\320\276\320\272\320\260", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "\320\235\320\276\320\262\320\260\321\217 \321\201\321\202\321\200\320\276\320\272\320\260", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->verticalHeaderItem(3);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "\320\235\320\276\320\262\320\260\321\217 \321\201\321\202\321\200\320\276\320\272\320\260", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->verticalHeaderItem(4);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "\320\235\320\276\320\262\320\260\321\217 \321\201\321\202\321\200\320\276\320\272\320\260", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->verticalHeaderItem(5);
        ___qtablewidgetitem8->setText(QApplication::translate("MainWindow", "\320\235\320\276\320\262\320\260\321\217 \321\201\321\202\321\200\320\276\320\272\320\260", Q_NULLPTR));

        const bool __sortingEnabled = tableWidget->isSortingEnabled();
        tableWidget->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->item(0, 0);
        ___qtablewidgetitem9->setText(QApplication::translate("MainWindow", "d:\\projects\\gpAll\\\320\244\320\221. \320\240\320\232\320\224. \320\237\321\200\320\265\321\201\320\265\320\273\320\265\320\272\321\202\320\276\321\200 0040S. \320\222\320\265\320\264\320\276\320\274\320\276\321\201\321\202\321\214   v1.0.2.xlsx", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget->item(0, 1);
        ___qtablewidgetitem10->setText(QApplication::translate("MainWindow", "2", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget->item(0, 2);
        ___qtablewidgetitem11->setText(QApplication::translate("MainWindow", "25", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget->item(1, 0);
        ___qtablewidgetitem12->setText(QApplication::translate("MainWindow", "d:\\projects\\gpAll\\\320\244\320\221. \320\240\320\232\320\224. \320\237\321\200\320\265\321\201\320\265\320\273\320\265\320\272\321\202\320\276\321\200 0040S. \320\222\320\265\320\264\320\276\320\274\320\276\321\201\321\202\321\214   v1.0.2.xlsx", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget->item(1, 1);
        ___qtablewidgetitem13->setText(QApplication::translate("MainWindow", "1", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget->item(1, 2);
        ___qtablewidgetitem14->setText(QApplication::translate("MainWindow", "12", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem15 = tableWidget->item(2, 0);
        ___qtablewidgetitem15->setText(QApplication::translate("MainWindow", "d:\\projects\\gpAll\\\320\244\320\221. \320\240\320\232\320\224. \320\237\321\200\320\265\321\201\320\265\320\273\320\265\320\272\321\202\320\276\321\200 0040S. \320\222\320\265\320\264\320\276\320\274\320\276\321\201\321\202\321\214   v1.0.2.xlsx", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem16 = tableWidget->item(2, 1);
        ___qtablewidgetitem16->setText(QApplication::translate("MainWindow", "2", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem17 = tableWidget->item(2, 2);
        ___qtablewidgetitem17->setText(QApplication::translate("MainWindow", "13", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem18 = tableWidget->item(3, 0);
        ___qtablewidgetitem18->setText(QApplication::translate("MainWindow", "d:\\projects\\gpAll\\\320\244\320\221. \320\240\320\232\320\224. \320\237\321\200\320\265\321\201\320\265\320\273\320\265\320\272\321\202\320\276\321\200 0040S. \320\222\320\265\320\264\320\276\320\274\320\276\321\201\321\202\321\214   v1.0.2.xlsx", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem19 = tableWidget->item(3, 1);
        ___qtablewidgetitem19->setText(QApplication::translate("MainWindow", "3", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem20 = tableWidget->item(3, 2);
        ___qtablewidgetitem20->setText(QApplication::translate("MainWindow", "15", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem21 = tableWidget->item(4, 0);
        ___qtablewidgetitem21->setText(QApplication::translate("MainWindow", "d:\\projects\\gpAll\\\320\244\320\221. \320\240\320\232\320\224. \320\237\321\200\320\265\321\201\320\265\320\273\320\265\320\272\321\202\320\276\321\200 0040S. \320\222\320\265\320\264\320\276\320\274\320\276\321\201\321\202\321\214   v1.0.2.xlsx", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem22 = tableWidget->item(4, 1);
        ___qtablewidgetitem22->setText(QApplication::translate("MainWindow", "4", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem23 = tableWidget->item(4, 2);
        ___qtablewidgetitem23->setText(QApplication::translate("MainWindow", "35", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem24 = tableWidget->item(5, 0);
        ___qtablewidgetitem24->setText(QApplication::translate("MainWindow", "d:\\projects\\gpAll\\\320\244\320\221. \320\240\320\232\320\224. \320\237\321\200\320\265\321\201\320\265\320\273\320\265\320\272\321\202\320\276\321\200 0040S. \320\222\320\265\320\264\320\276\320\274\320\276\321\201\321\202\321\214   v1.0.3.xlsx", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem25 = tableWidget->item(5, 1);
        ___qtablewidgetitem25->setText(QApplication::translate("MainWindow", "3", Q_NULLPTR));
        QTableWidgetItem *___qtablewidgetitem26 = tableWidget->item(5, 2);
        ___qtablewidgetitem26->setText(QApplication::translate("MainWindow", "30", Q_NULLPTR));
        tableWidget->setSortingEnabled(__sortingEnabled);

        pushButton_4->setText(QApplication::translate("MainWindow", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\320\237\321\200\320\276\321\206\320\265\321\201\321\201 \320\262\321\213\320\277\320\276\320\273\320\275\320\265\320\275\320\270\321\217", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("MainWindow", "\320\263\320\265\320\275\320\265\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214 \321\201\320\277\320\270\321\201\320\276\320\272\n"
"\320\277\320\276 partNumber", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("MainWindow", "\320\222\321\213\320\261\321\200\320\260\321\202\321\214 \321\204\320\260\320\271\320\273 \321\201\320\272\320\273\320\260\320\264\320\260", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindow", "\320\241\320\277\321\200\320\260\320\262\320\272\320\260", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
