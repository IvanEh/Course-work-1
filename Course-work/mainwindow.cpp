#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <time.h>
#include "systemsolver.h"
#include "gaussseidelsolver.h"
#include "jacobisolver.h"
#include "gaussianelimination.h"
#include "sorsolver.h"
#include <vector>
#include <aboutdialog.h>
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
//    QGridLayout* layout = new QGridLayout();
//    layout->addWidget(ui->labelSLAR, );
    
//    ui->widget_8->setLayout();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_pressed()
{

//    ui->nEdit->setStyleSheet("background-color: yellow");
    inputState = InputState::USER;
    resizeSystemView();
}


void MainWindow::resizeSystemView()
{

        QString nString = ui->nEdit->text();
        bool* ok = new bool;
        *ok = true;
        int n = nString.toInt(ok);
        int prevN = matrix.size();
        *ok = *ok && n >= 0;

        if(! *ok){
            ui->nEdit->setStyleSheet("QLineEdit { background-color: rgba(180, 0, 0, 64) }");
//            ui->nEdit->setAutoFillBackground( true );

            return;
        }else{
            ui->nEdit->setStyleSheet("QLineEdit {background: white}");
        }
    //    n  = 3;

        if(inputState == InputState::USER){
            updateSystemModelFromTable();
            resizeSystemModel(n);
            ui->linearSystemInput->setEnabled(true);
        }else{
            ui->linearSystemInput->setEnabled(false);
        }

        updateSystemView();
}

void MainWindow::clearSystemTable()
{
    int n = matrix.size();

    for(int i = 0; i < n; i++){
        for(int j= 0; j < n; j++){
            matrix[i][j] = 0;
        }
    }

    for(int i = 0; i < n; i++){
        vec[i] = 0;
    }

    updateSystemView();

}

void MainWindow::fillSystemModel()
{
    qsrand(time(nullptr));

    int n = matrix.size();

    for(int i = 0; i < n; i++){
        for(int j= 0; j < n; j++){
            double rand = qrand() % 25 - 12;
            if(i == j){
                rand*=3.5;
            }
             matrix[i][j] = rand;
        }
    }

     for(int i = 0; i < n; i++){
        double rand = qrand() % 70 - 30;
        vec[i] = rand;
     }

     ui->resultTable->setEditTriggers(QTableWidget::NoEditTriggers);
}

void MainWindow::updateSystemModelFromTable()
{
   int n = ui->linearSystemInput->rowCount();
   if(!validateSystem())
       return;

//   matrix.resize(n);
   for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            matrix[i][j] = ui->linearSystemInput->item(i, j)->text().toDouble();
        }
   }

   vec.resize(n);
   for(int i = 0; i < n; i++){
       vec[i] = ui->linearSystemInput->item(i, n)->text().toDouble();
   }
}

void MainWindow::fillDominantSystemModel()
{
    qsrand(time(nullptr));

    int n = matrix.size();

    for(int i = 0; i < n; i++){
        for(int j= 0; j < n; j++){
            double rand = qrand() % 25 - 12;
             matrix[i][j] = rand;
        }
    }

     for(int i = 0; i < n; i++){
        double rand = qrand() % 70 - 30;
        vec[i] = rand;
     }

    for(int i = 0; i < n; i++){
        int s = 0;
        for(int j = 0; j < n; j++){
            if(j != i)
                s += fabs(matrix[i][j]);
        }
        double q;
        if(qrand() % 2 == 0)
            q = 1;
        else
            q = -1;

        q = q + q*((qrand() % 15)/10.0);
        s = q*s;

        matrix[i][i] = s;
    }


}

bool MainWindow::calcExact()
{
   return setResultTable();
}

bool MainWindow::validateSystem()
{
    int n = ui->linearSystemInput->columnCount() - 1;
    bool valid = true;
    bool* ok = new bool;
    for(int i = 0; i < n && valid; i++){
        for(int j = 0; j < n; j++){
            QString cell = ui->linearSystemInput->item(i, j)->text();
            cell.toDouble(ok);
            if(! *ok){
                valid = false;
                ui->linearSystemInput->item(i,j)->setBackgroundColor(QColor(255, 0, 0));
                break;
            }
            ui->linearSystemInput->item(i,j)->setBackgroundColor(QColor(255, 255, 255));
        }
    }
    
    for(int i = 0; i < n && valid; i++){
        j = n;
            QString cell = ui->linearSystemInput->item(i, j)->text();
            cell.toDouble(ok);
            if(! *ok){
                valid = false;
                ui->linearSystemInput->item(i,j)->setBackgroundColor(QColor(255, 0, 0));
                break;
            }
            ui->linearSystemInput->item(i,j)->setBackgroundColor(QColor(255, 255, 255));
    }
    
    delete ok;

    return valid;
//    return true;
}

/*
 * retrieves linear system from tableview
 */
void MainWindow::setLinearSystem()
{
    int n;

    if(inputState == InputState::USER){
        if(!validateSystem())
            return;

        n = ui->linearSystemInput->columnCount()-1;

        if(n == -1)
            return;
        
        matrix = DMatrix(n);
        vec = DVector(n);

        for(int i =0; i < n; i++){
            matrix[i].resize(n);
        }

        ui->linearSystemInput->setEnabled(true);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                QString cell = ui->linearSystemInput->item(i, j)->text();
                double val = cell.toDouble();
                matrix[i][j] = val;
            }
        }

        int j = n;
        for(int i = 0; i < n; i++){
            QString cell = ui->linearSystemInput->item(i, j)->text();
            double val = cell.toDouble();
            vec[i] = val;
        }

       sys = LinearSystem(n, matrix, vec);
    }else{
        ui->linearSystemInput->setEnabled(false);
       // ui->linearSystemInput->clear();

        sys = LinearSystem(matrix.size(), matrix, vec);
        resizeSystemView();
    }


}

bool MainWindow::setResultTable()
{
    int n = matrix.size();
    ui->resultTable->setRowCount(n);
    ui->resultTable->setColumnCount(4);

    QStringList labels = QStringList({"Точний розв'язок", "Метод Якобі", "Метод Гауса-Зейделя", "Метод релаксації"});
    ui->resultTable->setHorizontalHeaderLabels(labels);

    GaussianElimination solver = GaussianElimination(sys);

    QElapsedTimer timer{};
    long elapsedTime = 0;

    timer.start();

    solver.solve();

    elapsedTime = timer.nsecsElapsed()/1000;
    ui->time4->setText(QString::number(elapsedTime).append(" мк"));
    bool succ = true;
    if(solver.isConvergent()){
        auto X = solver.getSolution();
        int j = 0;
        for(int i = 0; i < X.size(); i++){
            auto item = new QTableWidgetItem();
            item->setText(QString::number(X[i]));
            item->setBackgroundColor(normalCellColor);
            ui->resultTable->setItem(i, j, item);
        }
    }else{
        auto X = solver.getSolution();
        int j = 0;
        for(int i = 0; i < X.size(); i++){
            auto item = new QTableWidgetItem();
            item->setText("");
            item->setBackgroundColor(shadedCellColor);
            ui->resultTable->setItem(i, j, item);
        }
        succ = false;
    }

    ui->resultTable->setEditTriggers(QTableWidget::NoEditTriggers);
    
    return succ;

}

void MainWindow::calcIterationAlg()
{
    if(!validateSystem())
        return;
    
    int iter = 1;
    double eps = 0;
    double relaxFactor;
    int elapsedTime;
    
    bool iterChecked;
    bool ok = true;

    QElapsedTimer timer;

    iterChecked = ui->iterRadio->isChecked();
    eps = ui->precEdit->text().toDouble(&ok);
    if(!ok || eps <= 0){
        ui->precEdit->setStyleSheet("background: red");
        if(!iterChecked)
            return;
    }else{
        ui->precEdit->setStyleSheet("background: white");
    }

    ok = true;
    iter = ui->iterEdit->text().toInt(&ok);
    if(!ok || iter < 0){
        ui->iterEdit->setStyleSheet("background: red");
        if(iterChecked)
            return;
    }else{
        ui->iterEdit->setStyleSheet("background: white");
    }

    ok = true;
    relaxFactor = ui->relaxFactorEdit->text().toDouble(&ok);
    if(!ok || !(relaxFactor > 0 && relaxFactor < 2)){
        ui->relaxFactorEdit->setStyleSheet("background: red");
        return;
    }else{
        ui->relaxFactorEdit->setStyleSheet("background: rgb(230, 230, 230)");
    }

    bool succ = calcExact();
    int n = sys.getVariablesCount();

    JacobiSolver solver1 = JacobiSolver(sys);
    
    int iter1 = iter;
    elapsedTime = 0;
    bool convergent = solver1.isConvergent() && succ;
    if(convergent){

        timer.restart();
        long t1 = clock();
        if(iterChecked){
            eps = solver1.execIter(iter);
        }else{
            iter = solver1.solve(eps);
        }
        long t2 = clock();
        elapsedTime = timer.nsecsElapsed()/1000;

        auto X = solver1.getSolution();
        int j = 1;
        for(int i = 0; i < X.size(); i++){
            auto item = new QTableWidgetItem();

            if(!solver1.failed()){
                item->setText(QString::number(X[i]));
                item->setBackgroundColor(normalCellColor);
            }else{
                item->setText("");
                item->setBackgroundColor(shadedCellColor);
            }

            ui->resultTable->setItem(i, j, item);
        }
    }else{
        int j = 1;
        for(int i = 0; i < n; i++){
            auto item = new QTableWidgetItem();
            item->setText("");
            item->setBackgroundColor(shadedCellColor);
            ui->resultTable->setItem(i, j, item);
        }
    }

    ui->iter1->setText(QString::number(iter));
    ui->prec1->setText(QString::number(eps));
    ui->time1->setText(QString::number(elapsedTime).append(" мкc"));

    elapsedTime = 0;
    GaussSeidelSolver solver2 = GaussSeidelSolver(sys);
    convergent = solver2.isConvergent();
    if(convergent && succ){

        timer.start();
        if(iterChecked){
            eps = solver2.execIter(iter);
        }else{
            iter = solver2.solve(eps);
        }
        elapsedTime = timer.nsecsElapsed()/1000;

        auto X = solver2.getSolution();
        int j = 2;
        for(int i = 0; i < X.size(); i++){
            auto item = new QTableWidgetItem();

            if(!solver2.failed()){
                item->setText(QString::number(X[i]));
                item->setBackgroundColor(normalCellColor);
            }else{
                item->setText("");
                item->setBackgroundColor(shadedCellColor);
            }

            ui->resultTable->setItem(i, j, item);
        }
    }else
    {
        int j = 2;
        for(int i = 0; i < n; i++){
            auto item = new QTableWidgetItem();
            item->setText("");
            item->setBackgroundColor(shadedCellColor);
            ui->resultTable->setItem(i, j, item);
        }
    }

    ui->iter2->setText(QString::number(iter));
    ui->prec2->setText(QString::number(eps));
    ui->time2->setText(QString::number(elapsedTime).append(" мкc"));

    SORSolver solver3 = SORSolver(sys);
    solver3.setRelaxationFactor(relaxFactor);

    timer.start();
    if(iterChecked){
        eps = solver3.execIter(iter);
    }else{
        iter = solver3.solve(eps);
    }
     elapsedTime = timer.nsecsElapsed()/1000;
    if(solver3.converged() && succ){
        auto X = solver3.getSolution();
        int j = 3;
        for(int i = 0; i < X.size(); i++){
            auto item = new QTableWidgetItem();

            if(!solver3.failed()){
                item->setText(QString::number(X[i]));
                item->setBackgroundColor(normalCellColor);
            }else{
                item->setText("");
                item->setBackgroundColor(shadedCellColor);
            }

            ui->resultTable->setItem(i, j, item);
        }
    }else{
        int j = 3;
        for(int i = 0; i < n; i++){
            auto item = new QTableWidgetItem();
            item->setText("");
            item->setBackgroundColor(shadedCellColor);
            ui->resultTable->setItem(i, j, item);
        }
    }
    ui->iter3->setText(QString::number(iter));
    ui->prec3->setText(QString::number(eps));
    ui->time3->setText(QString::number(elapsedTime).append(" мкc"));




}

/**
 *
 * @brief MainWindow::resizeSystemModel
 * @param n
 */
void MainWindow::resizeSystemModel(int n)
{
    int prevN = matrix.size();

    DMatrix _matrix = DMatrix(matrix);
    DVector _vec = DVector(vec);

    _matrix.resize(n);
    for(int i = 0; i < n; i++)
        _matrix[i].resize(n);

    _vec.resize(n);

    if(n > prevN){
        int j;

        // restore after equal sign
        for(int i = 0; i <  prevN; i++){
            _vec[i] = vec[i];
        }


        // fill other after equal sign
        for(int i = prevN; i < n; i++){
           _vec[i] = 0;
        }

        // extend coeffs
        for(int i = prevN; i < n; i++){
            for(int j= prevN; j < n; j++){
               _matrix[i][j] = 0;
            }
        }
    }else{
            //restore after equal sign
        for(int i = 0; i < n; i++){
            _vec[i] = vec[i];
        }
    }

    matrix = _matrix;
    vec = _vec;
}

void MainWindow::updateSystemView()
{
    int lastN = ui->linearSystemInput->rowCount();
    int n = matrix.size();
    int modN = n;
    const int viewThreshold = 16;

    ui->linearSystemInput->setRowCount(modN);
    ui->linearSystemInput->setColumnCount(modN + 1);

    if(inputState == InputState::USER)
        ui->linearSystemInput->setEnabled(true);

    if(lastN != modN){
        if(modN < viewThreshold){
            int width = ui->linearSystemInput->width();
            int columnWidth = width / (modN+1);
            for(int i = 1; i <= modN+1; i++){
                ui->linearSystemInput->setColumnWidth(i-1, columnWidth);
            }
        }else{
            int width = ui->linearSystemInput->width();
            int columnWidth = width / viewThreshold;
            for(int i = 1; i <= modN+1; i++){
                ui->linearSystemInput->setColumnWidth(i-1, columnWidth);
            }
        }

    }

    for(int i = 0; i < modN; i++){
        QTableWidgetItem* item = new QTableWidgetItem;
        QString s = "X";
        s.append(QString::number(i + 1));
        s.append("");
        item->setText(s);
        ui->linearSystemInput->setHorizontalHeaderItem(i, item);
    }

    for(int i = 0; i < modN; i++){
     for(int j = 0; j < modN; j++){
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(QString::number(matrix[i][j]));
        ui->linearSystemInput->setItem(i, j, item);
     }
    }

    for(int i = 0; i < modN; i++){
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setText(QString::number(vec[i]));
        ui->linearSystemInput->setItem(i, n, item);
    }

     QTableWidgetItem* item = new QTableWidgetItem;
     item->setText("=");
     ui->linearSystemInput->setHorizontalHeaderItem(modN, item);
}

void MainWindow::on_pushButton_3_pressed()
{
    inputState = InputState::USER;
    clearSystemTable();
}

void MainWindow::on_pushButton_2_pressed()
{
    inputState = InputState::USER;
    fillSystemModel();
    updateSystemView();
}

void MainWindow::on_calcButton_pressed()
{
    setLinearSystem();
    calcIterationAlg();
}

void MainWindow::on_radioButton_toggled(bool checked)
{
    if(!checked)
        return;

    ui->precEdit->show();
    ui->iterEdit->setHidden(true);
}

void MainWindow::on_radioButton_2_toggled(bool checked)
{
    if(!checked)
        return;

    ui->iterEdit->show();
    ui->precEdit->setHidden(true);
}

void MainWindow::on_pushButton_4_pressed()
{
    inputState = InputState::USER;
    fillDominantSystemModel();
    updateSystemView();

}

void MainWindow::on_menuClose_triggered()
{
    this->close();
}

void MainWindow::on_menuSave_triggered()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, "Зберегти як");
    QFile file(fileName);
    if(file.exists()){
        QMessageBox dialog;
        dialog.setText("Файл буде перезаписано!");
        dialog.setIcon(QMessageBox::Warning);
        dialog.exec();
    }

    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    int n = ui->resultTable->rowCount();
    out << "Exact non-iterative result obtained by Gaussian elimination\r\n";
    for(int i = 0; i < n; i++){
        QString s = ui->resultTable->item(i, 0)->text();
        if(s.length() != 0){
            out << s << "\r\n";
        }
    }

    out << "Jacobi method's approximate result \r\n";
    for(int i = 0; i < n; i++){
        QString s = ui->resultTable->item(i, 1)->text();
        if(s.length() != 0){
            out << s << "\r\n";
        }
    }

    out << "Gauss-Seidel method's' approximate result \r\n";
    for(int i = 0; i < n; i++){
        QString s = ui->resultTable->item(i, 2)->text();
        if(s.length() != 0){
            out << s << "\r\n";
        }
    }

    out << "Successive Over-relaxation mathod's approximate result \r\n";
    for(int i = 0; i < n; i++){
        QString s = ui->resultTable->item(i, 3)->text();
        if(s.length() != 0){
            out << s << "\r\n";
        }
    }
    file.close();

}

void MainWindow::on_precRadio_toggled(bool checked)
{
    if(!checked)
        return;

    ui->precEdit->show();
    ui->iterEdit->setHidden(true);
}

void MainWindow::on_iterRadio_toggled(bool checked)
{
    if(!checked)
        return;

    ui->iterEdit->show();
    ui->precEdit->setHidden(true);
}

void MainWindow::on_openFile_triggered(bool checked)
{
    if(checked )
        return;

    QString fileName = QFileDialog::getOpenFileName();
    inputFile.setFileName(fileName);
    inputState = InputState::FILE;

    int n;

    inputFile.open(QIODevice::ReadOnly);
    QTextStream in(&inputFile);
    ui->linearSystemInput->setDisabled(true);

    bool fail = false;
    in >> n;
    fail = in.status() != QTextStream::Ok;

    if(fail){
        n = 0;
    }

    matrix = DMatrix(n);
    vec = DVector(n);

    for(int i =0; i < n && !fail; i++){
        matrix[i].resize(n);
    }

    for(int i = 0; i < n && !fail; i++){
        if(in.atEnd()){
            fail = true;
            break;
        }

        for(int j = 0; j < n ; j++){
            if(in.atEnd()){
                fail = true;
                break;
            }

            in >> matrix[i][j];
            fail = in.status() != QTextStream::Ok;
        }

        if(in.atEnd()){
            fail = true;
            break;
        }

        in >> vec[i];
        fail = in.status() != QTextStream::Ok;

    }

    sys = LinearSystem(n, matrix, vec);
    setResultTable();
    updateSystemView();

    if(fail){
        QMessageBox mess;
        mess.setText("Не можливо зчитати з файлу");
        mess.exec();
        inputFile.close();
        return;
    }else{
        QMessageBox mess;
        mess.setText("Зчитування пройшло успішно ");
        mess.exec();
    }

    inputFile.close();
}

void MainWindow::on_openFile_triggered()
{

}

void MainWindow::on_calcButton_clicked()
{

}

void MainWindow::on_about_triggered()
{
    AboutDialog about;
    about.exec();
}

void MainWindow::on_pushButton_released()
{

}
