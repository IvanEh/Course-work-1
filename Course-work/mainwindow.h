#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow> // Створення вікна
#include <QFile> // Доступ до файлової системи
#include "linear-system.h" // Представлення систем
namespace Ui {
class MainWindow;
}

enum class InputState {
    FILE, USER
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    LinearSystem sys;
    const QColor shadedCellColor = QColor(128, 128, 128, 128);
    const QColor normalCellColor = QColor(255, 255, 255);
    DMatrix matrix = DMatrix();
    DVector vec = DVector();
    InputState inputState = InputState::USER;
    QFile inputFile;
    //SystemLinearEquation sys = SystemLinearEquation();
private:
    void clearSystemTable();
    void fillSystemModel();

    /*!
     * retrieves coeffs from table view
     * and changes the global sys model
     */
    void updateSystemModelFromTable();
    /**
     * @brief fillDominantSystemModel
     * Заповнює модель СЛАР так, що вона буде з домінантною перевагою
     */
    void fillDominantSystemModel();
    /**
     * @brief calcExact
     * Вивести точний результат
     */
    bool calcExact();

    /**
     * @brief validateSystem
     * Перевірити на правильність вводу
     */
    bool validateSystem();

    /**
     * @brief setLinearSystem
     * Встановити модель та вигляд СЛАР
     */
    void setLinearSystem();

    /**
     * @brief setResultTable
     * Ініціалізувати таблицю результатів
     */
    bool setResultTable();

    /**
     * @brief calcIterationAlg
     * Обчислити й вивести результат
     */
    void calcIterationAlg();

    /**
     * @brief resizeSystemModel
     * змінити розмір матриці
     */
    void resizeSystemModel(int n);
    /**
     * @brief updateSystemView
     * Оновити вид матриці
     */
    void updateSystemView();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeSystemView();

private slots:
    void on_pushButton_pressed();

    void on_pushButton_3_pressed();

    void on_pushButton_2_pressed();

    void on_calcButton_pressed();

    void on_radioButton_toggled(bool checked);

    void on_radioButton_2_toggled(bool checked);

    void on_pushButton_4_pressed();

    void on_menuClose_triggered();

    void on_menuSave_triggered();

    void on_precRadio_toggled(bool checked);

    void on_iterRadio_toggled(bool checked);

    void on_openFile_triggered(bool checked);

    void on_openFile_triggered();

    void on_calcButton_clicked();

    void on_about_triggered();

    void on_pushButton_released();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
