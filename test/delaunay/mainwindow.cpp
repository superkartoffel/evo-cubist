// Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>
// All rights reserved.

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "main.h"
#include "painterpathclass.h"
#include "../../circle.h"
#include "../../random/rnd.h"
#include "../../helper.h"

#include <QtCore>
#include <QtCore/QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsItem>
#include <QPen>
#include <QColor>
#include <QBrush>
#include <QVector>
#include <QSettings>
#include <qmath.h>


MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::MainWindow)
    , mShowSplices(false)
    , mStopped(false)
{
    ui->setupUi(this);

    setFocusPolicy(Qt::StrongFocus);
    setFocus(Qt::OtherFocusReason);

    RAND::initialize();

    QScriptValue m = mScriptEngine.newQObject(this);
    mScriptEngine.globalObject().setProperty("MainWindow", m);

    PainterPathClass* ppClass = new PainterPathClass(&mScriptEngine);
    mScriptEngine.globalObject().setProperty("PainterPath", ppClass->constructor());

#ifndef QT_NO_DEBUG
    mDebugger.attachTo(&mScriptEngine);
    mDebugger.action(QScriptEngineDebugger::InterruptAction)->trigger();
    mDebugger.widget(QScriptEngineDebugger::DebugOutputWidget)->show();
#endif

    mScene.setSceneRect(0, 0, 1, 1);
    mScene.setItemIndexMethod(QGraphicsScene::NoIndex);
    mView.resize(640, 640);
    mView.fitInView(mScene.sceneRect());
    mView.setRenderHint(QPainter::Antialiasing);
    mView.scale(640, 640);
    mView.setBackgroundBrush(QColor(20, 20, 20));
    mView.setWindowTitle("Random Tiling Test");
    mView.setScene(&mScene);
    mView.show();

    QObject::connect(this, SIGNAL(tilingProgressed(int)), ui->progressBar, SLOT(setValue(int)));
    QObject::connect(this, SIGNAL(tilingProgressed(int)), &mScene, SLOT(update()), Qt::BlockingQueuedConnection);
    QObject::connect(&mTileThreadWatcher, SIGNAL(finished()), SLOT(tileThreadFinished()));
    QObject::connect(ui->runStopPushButton, SIGNAL(clicked()), SLOT(runStopScript()));
    QObject::connect(ui->executePushButton, SIGNAL(clicked()), SLOT(executeScript()));

    restoreSettings();

}


MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}


void MainWindow::restoreSettings(void)
{
    QSettings settings(Company, AppName);
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    mView.restoreGeometry(settings.value("GraphicsView/geometry").toByteArray());
    QFile script(":/scripts/default.js");
    script.open(QIODevice::ReadOnly | QIODevice::Text);
    QString scriptText = script.readAll();
    script.close();
    ui->scriptEditor->setPlainText(settings.value("MainWindow/script", scriptText).toString());
}


void MainWindow::saveSettings(void)
{
    QSettings settings(Company, AppName);
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("MainWindow/script", ui->scriptEditor->toPlainText());
    settings.setValue("GraphicsView/geometry", mView.saveGeometry());
}


void MainWindow::closeEvent(QCloseEvent* e)
{
    saveSettings();
    mView.close();
    mTileThread.cancel();
    mTileThread.waitForFinished();
    e->accept();
}


void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::black);
    painter.setBrush(Qt::green);
    painter.setPen(Qt::transparent);
    for (QPolygonF::const_iterator p = mPolygon.constBegin(); p != mPolygon.constEnd(); ++p)
        painter.drawEllipse(*p, 3.0, 3.0);
    if (mPolygon.size() == 3) {
        Circle circle(mPolygon.at(0), mPolygon.at(1), mPolygon.at(2));
        painter.setPen(QColor(255, 255, 0));
        painter.setBrush(QColor(255, 255, 0, 40));
        painter.drawEllipse(circle.center(), circle.radius(), circle.radius());
        painter.setBrush(QColor(255, 255, 0, 110));
        painter.drawEllipse(circle.center(), 1.65, 1.65);
    }
    if (isConvexPolygon(mPolygon)) {
        painter.setPen(QColor(0, 255, 0));
        painter.setBrush(QColor(0, 255, 0, 40));
    }
    else {
        painter.setPen(QColor(255, 0, 0));
        painter.setBrush(QColor(255, 0, 0, 40));
    }
    painter.drawPolygon(mPolygon);

    if (mGene.isAlive()) {
        painter.setPen(QPen(QBrush(QColor(255, 0, 0)), 3, Qt::SolidLine));
        painter.setBrush(QColor(255, 0, 0, 80));
        painter.drawPolygon(mGene.polygon());
        painter.setPen(QPen(QBrush(QColor(255, 0, 255)), 1, Qt::SolidLine));
        if (mSplices.size() > 0 && mShowSplices) {
            painter.setBrush(Qt::transparent);
            painter.setPen(QPen(QBrush(Qt::white), 1.5, Qt::DashDotLine));
            for (QVector<Gene>::const_iterator g = mSplices.constBegin(); g != mSplices.constEnd(); ++g)
                painter.drawPolygon(g->polygon());
        }
    }

    painter.setPen(QColor(255, 255, 255, 200));
    painter.drawText(QRectF(width() - 210, 5, 200, 80), Qt::AlignRight | Qt::AlignTop,
                     "GENERATED CONVEX POLYGON\r\n"
                     "S: show/hide splices\r\n"
                     "R: generate random polygon\r\n"
                     "C: clear polygon");
    painter.drawText(QRectF(width() - 210, height()-85, 200, 80), Qt::AlignRight | Qt::AlignBottom,
                     "MANUALLY PRODUCED POLYGON\r\n"
                     "left click: add point\r\n"
                     "right click: clear\r\n");
}


void MainWindow::mousePressEvent(QMouseEvent* e)
{
    switch (e->button()) {
    case Qt::LeftButton:
        mPolygon.append(e->posF());
        break;
    case Qt::RightButton:
        mPolygon.clear();
        break;
    }
    e->accept();
    update();
}


void MainWindow::tileThreadFinished(void)
{
    mScene.update();
    mView.setCursor(Qt::ArrowCursor);
    ui->runStopPushButton->setText(tr("Run"));
    ui->progressBar->setValue(0);
    ui->progressBar->setEnabled(false);
}


void MainWindow::startTiling(void)
{
    if (!mTileThread.isRunning()) {
        mView.setCursor(Qt::WaitCursor);
        mTileThread = QtConcurrent::run(this, &MainWindow::tileRandomly);
        mTileThreadWatcher.setFuture(mTileThread);
        ui->progressBar->setEnabled(true);
    }
}


void MainWindow::runStopScript(void)
{
    if (ui->runStopPushButton->text() == tr("Run")) {
        mStopped = false;
        mScriptEngine.evaluate(ui->scriptEditor->toPlainText());
        if (mScriptEngine.hasUncaughtException()) {
            qWarning() << mScriptEngine.uncaughtExceptionBacktrace() << mScriptEngine.uncaughtException().toString();
        }
        else {
            ui->runStopPushButton->setText(tr("Stop"));
        }
    }
    else {
        mStopped = true;
    }
}


void MainWindow::executeScript(void)
{
    mScriptEngine.evaluate(ui->scriptEditor->toPlainText());
    if (mScriptEngine.hasUncaughtException()) {
        qWarning() << mScriptEngine.uncaughtExceptionBacktrace() << mScriptEngine.uncaughtException().toString();
    }
}


void MainWindow::tileRandomly(void)
{
    // see http://paulbourke.net/texture_colour/randomtile/
    static const QPen pen(Qt::transparent);
    static const QBrush brush[3] =
    {
        QBrush(QColor(255, 0, 0, 200)),
        QBrush(QColor(255, 255, 0, 200)),
        QBrush(QColor(0, 255, 0, 200)),
    };
    mScene.clear();
    QScriptValue vN = mScriptEngine.globalObject().property("N");
    if (vN.isUndefined())
        return;
    const int N = vN.toInt32();
    QScriptValue vMaxTrials = mScriptEngine.globalObject().property("MAX_TRIALS");
    if (vMaxTrials.isUndefined())
        return;
    const int MAX_TRIALS =  vMaxTrials.toInt32();
    QScriptValue proceed = mScriptEngine.globalObject().property("proceed");
    if (proceed.isUndefined()) {
        qWarning() << "proceed.isUndefined()";
        return;
    }
    QScriptValue getShape = mScriptEngine.globalObject().property("getShape");
    if (getShape.isUndefined()) {
        qWarning() << "getShape.isUndefined()";
        return;
    }
    while (mScene.items().size() < N) {
        proceed.call();
        int numTrials = 0;
        bool fitting = false;
        while (!fitting && numTrials < MAX_TRIALS) {
            if (mStopped)
                goto end;
            ++numTrials;
            QScriptValueList vShapeArgs;
            vShapeArgs << QScriptValue(RAND::rnd1()) << QScriptValue(RAND::rnd1());
            QScriptValue vShape = getShape.call(QScriptValue(), vShapeArgs);
            QPainterPath path = qscriptvalue_cast<QPainterPath>(vShape);
            bool colliding = false;
            const QList<QGraphicsItem*>& items = mScene.items();
            for (QList<QGraphicsItem*>::const_iterator it = items.constBegin(); it != items.constEnd(); ++it) {
                if ((*it)->collidesWithPath(path)) {
                    colliding = true;
                    break;
                }
            }
            if (!colliding) {
                fitting = true;
                mScene.addPath(path, pen, brush[mScene.items().size() % 3]);
            }
            else {
                fitting = false;
            }
        }
        emit tilingProgressed(ui->progressBar->maximum() * mScene.items().size() / N);
    }
end:;
}


void MainWindow::keyPressEvent(QKeyEvent* e)
{
    switch (e->key()) {
    case Qt::Key_Escape:
    {
        mPolygon.clear();
        mGene = Gene();
        update();
        break;
    }
    case Qt::Key_R:
    {
        QPolygonF polygon;
        const int N = RAND::rnd(3, 6);
        for (int i = 0; i < N; ++i)
            polygon << QPointF(RAND::rnd1(10, width()-10), RAND::rnd1(10, height()-10));
        polygon = convexHull(polygon);
        mGene = Gene(polygon, Qt::black);
        mSplices = mGene.splice();
        mPolygon.clear();
        update();
        break;
    }
    case Qt::Key_S:
    {
        mShowSplices = !mShowSplices;
        update();
        break;
    }
    case Qt::Key_C:
    {
        mGene = Gene();
        update();
        break;
    }
    default:
        break;
    }
}
