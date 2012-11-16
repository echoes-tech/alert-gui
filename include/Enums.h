/* 
 * File:   Enums.h
 * Author: tsa
 *
 * Created on 14 septembre 2012, 15:19
 */

#ifndef ENUMS_H
#define	ENUMS_H

class Enums {
public:
    Enums();
    Enums(const Enums& orig);
    virtual ~Enums();
    enum EMedia
    {
        email = 1, 
        sms = 2
    };
    enum EAction
    {
        login = 1, 
        registration = 2, 
        logout = 3,
        display = 4,
        download = 5,
        add = 6,
        del = 7
    };
    enum EInformationUnitType
    {
        text = 1, 
        number = 2
    };
    enum EOption
    {
        quotaAsset = 1,
        quotaSms = 2
    };
private:

};

#endif	/* ENUMS_H */

