#ifndef DATASTORAGE_H
#define DATASTORAGE_H


#include <QMap>

struct TData{
QString part;
QList<int> counts;
};

class dataStorage{
    QMap <QString, QList<TData>> storage_;
public:


    QMap <QString, QList<TData>> result(){

    }

    void insert(QString refDez, QString partNum, int qty, int partNumbCount, int percent, int numberBom ){
        numberBom++; //т.к. 0 это первый BOM
        int c = refDez.indexOf(",");
        if(c > 0){
            refDez.remove(c, refDez.count() - c);
        }
        c = refDez.indexOf("-");
        if(c > 0){
            refDez.remove(c, refDez.count() - c);
        }
        refDez.remove(QRegExp("[^A-Za-zА-Яа-я]"));

        int countPart = qty * partNumbCount;
        int countPartpERCENT = qty * partNumbCount + ceil(qty * partNumbCount*percent/100);
        // Есть ли такой ключ?
        bool keyF = false;
        foreach (auto var, storage_.keys()) {
            if( var == refDez){
                keyF = true;
                break;
            }
        }
        int k = 0; // для отладки
        //Такой ключ уже существует
        if(keyF == true){
            bool PartNumbF = false;//есть ли такой partNumber?
            for (int i = 0; i < storage_[refDez].count(); i++) {
                int c = storage_[refDez].count(); //сколько
                QString a = storage_[refDez].at(i).part;
                if( a == partNum){
                    int num = storage_[refDez].at(i).counts.count(); //сколько эл-тов?
                    //заполнить все Qlist<int> по всем ключам 0
                    if(num < numberBom){

                        // добавить нули во все остальные позиции для всех partNumber по всем ключам
                        foreach (auto key, storage_.keys()) {
                            for(int i = 0; i < storage_[key].count(); i++){
                                //сколько эл-тов Qlist<int>
                                int c = storage_[key][i].counts.count();
                                if( c < numberBom )
                                    storage_[key][i].counts << 0;

                            }
                        }


                    }
                    num = storage_[refDez].at(i).counts[numberBom-1];
                    storage_[refDez][i].counts[numberBom-1] += countPart;
                    PartNumbF = true;
//                    break;
                }
            }
            // такого partNumber'а нет
            if( PartNumbF == false ){
                QList<TData> bomNumb;
                TData strAndNumb;
                strAndNumb.part = partNum;
                for(int i = 0; i < numberBom; i++){
                    if(i == numberBom - 1){
                        strAndNumb.counts << countPart;
                        break;
                    }
                    strAndNumb.counts << 0;

                }
                bomNumb << strAndNumb;
                storage_[refDez].append(bomNumb);
                // добавить нули во все остальные позиции для всех partNumber по всем ключам
                foreach (auto key, storage_.keys()) {
                    for(int i = 0; i < storage_[key].count(); i++){
                        //сколько эл-тов Qlist<int>
                        int c = storage_[key][i].counts.count();
                        if( c < numberBom )
                            storage_[key][i].counts << 0;

                    }
                }
                k++;
            }


                k++;
            }

        //такого ключа нет
        else{
            QList<TData> bomNumb;
            TData strAndNumb;
            strAndNumb.part = partNum;
            for(int i = 0; i < numberBom; i++){
                if(i == numberBom - 1){
                    strAndNumb.counts << countPart;
                    break;
                }
                strAndNumb.counts << 0;

            }
            bomNumb << strAndNumb;
            storage_.insert(refDez,bomNumb );
            k++;

        }
        //storage_.insert(refDez, )
    }
    QMap <QString, QList<TData> >  ret() {
        return storage_;
    }

};

#endif // DATASTORAGE_H
