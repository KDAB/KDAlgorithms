/****************************************************************************
**
** This file is part of KDAlgorithms
**
** SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
**
** SPDX-License-Identifier: MIT
**
****************************************************************************/

#include "../src/kdalgorithms.h"
#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QTableWidget>
#include <QVBoxLayout>

/*
    For details see https://github.com/KDAB/KDAlgorithms/blob/main/Documentation/inspiration.md
*/
class Test : public QWidget
{
public:
    Test()
    {
        auto layout = new QVBoxLayout(this);

        m_table = new QTableWidget(5, 5);
        layout->addWidget(m_table);

        m_label1 = new QLabel("Select Something");
        layout->addWidget(m_label1);
        connect(m_table, &QTableWidget::currentCellChanged, this, &Test::updateLabelV1);

        m_label2 = new QLabel("Select Something");
        layout->addWidget(m_label2);
        connect(m_table, &QTableWidget::currentCellChanged, this, &Test::updateLabelV2);

        m_label3 = new QLabel("Select Something");
        layout->addWidget(m_label3);
        connect(m_table, &QTableWidget::currentCellChanged, this, &Test::updateLabelV3);
    }

private:
    /* The normal implementation using for loops */
    void updateLabelV1()
    {
        bool allColumnsSelected = true;
        int row = m_table->currentRow();
        auto selectionModel = m_table->selectionModel();
        auto model = m_table->model();
        for (int i = 0; i < m_table->columnCount(); ++i) {
            if (!selectionModel->isSelected(model->index(row, i))) {
                allColumnsSelected = false;
                break;
            }
        }

        if (allColumnsSelected)
            m_label1->setText("All columns selected");
        else
            m_label1->setText("Not all columns selected");
    }

    /* The improved version using algorithms */
    void updateLabelV2()
    {
        auto columns = kdalgorithms::iota(m_table->columnCount());
        auto isColumnSelected = [row = m_table->currentRow(),
                                 selectionModel = m_table->selectionModel(),
                                 model = m_table->model()](int column) {
            return selectionModel->isSelected(model->index(row, column));
        };

        if (kdalgorithms::all_of(columns, isColumnSelected))
            m_label2->setText("All columns selected");
        else
            m_label2->setText("Not all columns selected");
    }

    /* The version I would have written without algorithms */
    void updateLabelV3()
    {
        bool allColumnsSelected = [this] {
            int row = m_table->currentRow();
            auto selectionModel = m_table->selectionModel();
            auto model = m_table->model();
            for (int i = 0; i < m_table->columnCount(); ++i) {
                if (!selectionModel->isSelected(model->index(row, i))) {
                    return false;
                }
            }
            return true;
        }();

        if (allColumnsSelected)
            m_label3->setText("All columns selected");
        else
            m_label3->setText("Not all columns selected");
    }

    QTableWidget *m_table;
    QLabel *m_label1;
    QLabel *m_label2;
    QLabel *m_label3;
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Test test;
    test.resize(600, 300);
    test.show();
    app.exec();
}
