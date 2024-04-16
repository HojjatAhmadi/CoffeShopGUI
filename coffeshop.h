#ifndef COFFESHOP_H
#define COFFESHOP_H

#include "ui_coffeshop.h"
#include <QMainWindow>
#include <QWidget>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QByteArray>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVector>
#include <QMessageBox>
#include <QSpinBox>
#include <QtMath>

QT_BEGIN_NAMESPACE
namespace Ui {
class CoffeShop;
}
QT_END_NAMESPACE

class CoffeShop : public QMainWindow
{
    Q_OBJECT

public:
    CoffeShop(QWidget *parent = nullptr);
    ~CoffeShop();
    QString selectedproduct[2][100];
    int numberProductSelect = 0;



    QJsonArray getAllProduct(){
        QDir appDir(QCoreApplication::applicationDirPath());
        QFile jsonFile = appDir.filePath("data.json");
        jsonFile.open(QIODevice::ReadOnly);

        QByteArray ReadAllData = jsonFile.readAll();
        QJsonDocument changeDataTOjson = QJsonDocument::fromJson(ReadAllData);
        QJsonObject jsonOBJ = changeDataTOjson.object();
        QJsonArray AllProduct = jsonOBJ["Product"].toArray();

        jsonFile.close();
        return AllProduct;
    }


    QFrame * creatCart(QString uniqid , QString name , QString price , QString number){
        QFrame *newFrame = new QFrame();
        newFrame->setFrameShape(QFrame::Box);
        newFrame->setLineWidth(1);


        QHBoxLayout *newBoxLayout = new QHBoxLayout(newFrame);


        QPushButton *clickButton = new QPushButton("", newFrame);
        clickButton->setObjectName(uniqid+"_"+"off");
        clickButton->setIcon(QIcon(":/icon/image/cartfile.png"));
        clickButton->setMaximumSize(20,20);
        newBoxLayout->addWidget(clickButton);

        QSpinBox *numberOfProduct = new QSpinBox(newFrame);
        numberOfProduct->setObjectName(uniqid+"_"+"SpinBox");
        newBoxLayout->addWidget(numberOfProduct);

        QLabel *priceL = new QLabel(this->formattedPrice(price));
        QLabel *nameL = new QLabel(name);
        QLabel *numberL = new QLabel("."+number);
        numberL->setAlignment(Qt::AlignRight | Qt::AlignCenter);
        priceL->setAlignment(Qt::AlignRight | Qt::AlignCenter);
        nameL->setAlignment(Qt::AlignRight | Qt::AlignCenter);
        newBoxLayout->addWidget(priceL);
        newBoxLayout->addWidget(nameL);
        newBoxLayout->addWidget(numberL);


        newFrame->setLayout(newBoxLayout);
        return newFrame;
    }


    QString formattedPrice(QString previousPrice){
        for (int i = previousPrice.length() - 3;i > 0;i -= 3){
            previousPrice.insert(i, ",");
        }
        return previousPrice;
    }


    bool removeAllProduct(){
        QList<QFrame *> allFrames = ui->ShowCart->findChildren<QFrame *>();
        for(QFrame *frame : allFrames){
            frame->deleteLater();
        }
        return true;
    }


    void showTypeOfProduct(QString type){
        this->removeAllProduct();
        QJsonArray allProduct = this->getAllProduct();
        int number = 1;
        foreach(QJsonValue value , allProduct){
            QJsonObject product = value.toObject();
            if(product["type"] == type){
                ui->scrollAreaWidgetContents->layout()->addWidget(this->creatCart(product["uniqid"].toString() , product["name"].toString() , product["price"].toString() , QString::number(number++)));
            }
        }
        QMessageBox::information(this , "User Notif" , "List Updated!");
    }

    void printAllProductCart(){
        for(int i=0 ; i<2 ; i++){
            for(int j=0 ; j<this->numberProductSelect ; j++){
                qDebug() << this->selectedproduct[i][j];
            }
        }
    }

    void updatePeoductCart(QString buttonObject , bool set){
        QStringList objectBtn = buttonObject.split("_");
        if(set){
            // set product
            QList <QSpinBox *> allSpinBox = ui->ShowCart->findChildren<QSpinBox *>();
            for(QSpinBox *spinBox:allSpinBox){
                QStringList objectSpin = spinBox->objectName().split("_");
                if(objectSpin[0] == objectBtn[0]){
                    this->selectedproduct[0][this->numberProductSelect] = objectSpin[0];
                    this->selectedproduct[1][this->numberProductSelect] = spinBox->text();
                    this->numberProductSelect++;
                    break;
                }
            }
        }
        else{
            // delete product
            for(int i=0 ; i<2 ; i++){
                for(int j=0 ; j<this->numberProductSelect ; j++){
                    if(this->selectedproduct[i][j] == objectBtn[0]){
                        for(int k=j ; k<this->numberProductSelect-1 ; k++){
                            this->selectedproduct[i][k] = this->selectedproduct[i][k+1];
                        }
                        this->numberProductSelect--;
                        break;
                    }
                }
            }
        }
    }


    void ButtonFrameEvent(QPushButton *button){
        QString icon = button->icon().name();
        if(button->objectName().contains("off")){
            // Setproduct on Array
            this->updatePeoductCart(button->objectName() , true);
            button->setIcon(QIcon(":/icon/image/icons8-clear-shopping-cart-60.png"));
            button->setObjectName(button->objectName().replace("off" , "on"));
        }
        else{
            // deleteproduct from Array
            this->updatePeoductCart(button->objectName() , false);
            button->setIcon(QIcon(":/icon/image/cartfile.png"));
            button->setObjectName(button->objectName().replace("on" , "off"));
        }
    }


    void saveAllSpinData(){
        QList<QSpinBox *> allSpinBox = ui->ShowCart->findChildren<QSpinBox *>();
        for(QSpinBox *spinBox:allSpinBox){
            QStringList objectSpin = spinBox->objectName().split("_");
            for(int i=0 ; i<1 ; i++){
                for(int j=0 ; j<this->numberProductSelect ; j++){
                    if(this->selectedproduct[i][j] == objectSpin[0]){
                        spinBox->setValue(this->selectedproduct[1][j].toInt());
                    }
                }
            }
        }
    }

    void saveAllBtnStyle(){
        QList<QPushButton *> allButton = ui->ShowCart->findChildren<QPushButton *>();
        for(QPushButton *btn:allButton){
            QStringList objectBtn = btn->objectName().split("_");
            for(int i=0 ; i<1 ; i++){
                for(int j=0 ; j<this->numberProductSelect ; j++){
                    if(this->selectedproduct[i][j] == objectBtn[0]){
                        btn->setIcon(QIcon(":/icon/image/icons8-clear-shopping-cart-60.png"));
                        btn->setObjectName(btn->objectName().replace("off" , "on"));
                    }
                }
            }
        }
    }


    void getAllBtn(){
        this->saveAllSpinData();
        this->saveAllBtnStyle();
        QList <QFrame *> allbtnFrames = ui->ShowCart->findChildren<QFrame *>();
        int i=0;
        for(QFrame *frame : allbtnFrames){
            foreach (QPushButton *button, frame->findChildren<QPushButton *>()) {
                connect(button, &QPushButton::clicked, this, [=]() {
                    this->ButtonFrameEvent(button);
                });
            }
            i++;
        }
    }


    QString findProductMoney(QString uniqid){
        QJsonArray allProduct = this->getAllProduct();
        foreach (QJsonValue value, allProduct) {
            QJsonObject product = value.toObject();
            if(product["uniqid"] == uniqid){
                return product["price"].toString();
            }
        }
    }


    void showallMoney(){
        int money = 0;
        for(int i=0 ; i<1 ; i++){
            for(int j=0 ; j<this->numberProductSelect ; j++){
                money += (this->findProductMoney(this->selectedproduct[i][j]).toInt()*this->selectedproduct[1][j].toInt());
            }
        }
        ui->showAllMoney->setText(this->formattedPrice(QString::number(money)) + "$");
    }

private slots:
    void on_coldType_clicked();

    void on_hotType_clicked();

    void on_foodType_clicked();

    void on_AllProductType_clicked();

    void on_completePurchase_clicked();

    void on_pushButton_clicked();

private:
    Ui::CoffeShop *ui;
};
#endif // COFFESHOP_H
