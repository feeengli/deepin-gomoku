/*
   * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
   *
   * Author:     zhengxiaokang <zhengxiaokang@uniontech.com>
   *
   * Maintainer: zhengxiaokang <zhengxiaokang@uniontech.com>
   *
   * This program is free software: you can redistribute it and/or modify
   * it under the terms of the GNU General Public License as published by
   * the Free Software Foundation, either version 3 of the License, or
   * any later version.
   *
   * This program is distributed in the hope that it will be useful,
   * but WITHOUT ANY WARRANTY; without even the implied warranty of
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   * GNU General Public License for more details.
   *
   * You should have received a copy of the GNU General Public License
   * along with this program.  If not, see <http://www.gnu.org/licenses/>.
   */
#include "selectchess.h"
#include "../resultpopup/closepopup.h"
#include "selectinfo.h"
#include "determinebutton.h"
#include "chessselected.h"
#include "constants.h"

#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>

#include <DHiDPIHelper>

DWIDGET_USE_NAMESPACE
Selectchess::Selectchess(QWidget *parent)
    : QDialog(parent)
    , backgroundPixmap(DHiDPIHelper::loadNxPixmap(":/resources/chessselected/selectbase.svg"))
    , selectLButton(new Selectbutton)
    , selectRButton(new Selectbutton)
{
    setFixedSize(371, 219);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog); //设置flags隐藏标题栏

    initUI();
}

/**
 * @brief Selectchess::initUI 初始化界面
 */
void Selectchess::initUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QHBoxLayout *closeLayout = new QHBoxLayout();
    Closepopup *closeBt = new Closepopup();
    connect(closeBt, &Closepopup::signalCloseClicked, this, [ = ] {
        this->close();
    });
    closeLayout->addWidget(closeBt, 0, Qt::AlignRight);
    closeLayout->addSpacing(12);

    QHBoxLayout *seleceInfoLayout = new QHBoxLayout();
    Selectinfo *selectInfo = new Selectinfo();
    seleceInfoLayout->addStretch();
    seleceInfoLayout->addWidget(selectInfo);
    seleceInfoLayout->addStretch();

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    Chessselected *LChessSelected = new Chessselected(chess_white);
    Chessselected *RchessSelected = new Chessselected(chess_black);
    selectRButton->setChecked(true);
    //选择的那个颜色棋子, 发送信号
    buttonLayout->addStretch(70);
    buttonLayout->addWidget(selectLButton);
    buttonLayout->addWidget(LChessSelected);
    buttonLayout->addStretch(40);
    buttonLayout->addWidget(selectRButton);
    buttonLayout->addWidget(RchessSelected);
    buttonLayout->addStretch(70);

    QHBoxLayout *determineLayout = new QHBoxLayout();
    Determinebutton *determineButton = new Determinebutton();
    connect(determineButton, &Determinebutton::signalButtonOKClicked, this, &Selectchess::slotButtonOKClicked);
    determineLayout->addWidget(determineButton);

    mainLayout->addSpacing(8);
    mainLayout->addLayout(closeLayout);
    mainLayout->addLayout(seleceInfoLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(determineLayout);
    mainLayout->addSpacing(14);
    setLayout(mainLayout);
}

/**
 * @brief Selectchess::slotButtonOKClicked 确定按钮点击后发送信号
 */
void Selectchess::slotButtonOKClicked()
{
    //如果选择了棋子颜色, 发送信号, 否则不发送
    emit signalButtonOKClicked();
}

/**
 * @brief Selectchess::selectChessShow 展示弹窗
 */
void Selectchess::selectChessShow()
{
    this->exec();
}

/**
 * @brief Selectchess::setSelectChess 设置选择的棋子颜色
 * @param chessColor 棋子颜色
 */
void Selectchess::setSelectChess(int chessColor)
{
    //用户选择的棋子颜色
    selectChessColor = chessColor;
    //根据用户选择的棋子颜色, 设置不同按钮使能
    if (selectChessColor == chess_white) {
        selectLButton->setChecked(true);
    } else if (selectChessColor == chess_black) {
        selectRButton->setChecked(true);
    }
}

/**
 * @brief Selectchess::getSelsectChess 获取选择的棋子颜色
 * @return 棋子颜色
 */
int Selectchess::getSelsectChess()
{
    //根据按钮选择, 设置棋子颜色
    if (selectLButton->isChecked()) {
        selectChessColor = chess_white;
    } else if (selectRButton->isChecked()) {
        selectChessColor = chess_black;
    }
    return selectChessColor;
}

/**
 * @brief Selectchess::closeSelectPopup 关闭弹窗
 */
void Selectchess::slotCloseSelectPopup()
{
    this->close();
}

/**
 * @brief Selectchess::paintEvent 绘图函数
 * @param event paintevent
 */
void Selectchess::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.drawPixmap(this->rect(), backgroundPixmap);
    painter.restore();
    QDialog::paintEvent(event);
}