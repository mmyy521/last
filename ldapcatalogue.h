#ifndef LDAPCATALOGUE_H
#define LDAPCATALOGUE_H
#include <windows.h>
//#include <windef.h>
#include <winldap.h>
//#include <winber.h>
#include <QDebug>
#include <QString>

#include "basetreenode.h"

class LDAPCatalogue
{
public:
    LDAPCatalogue();
    BaseTreeNode* GetCatalogue(QString getHost,QString getUserName);
    BaseTreeNode* GetCatalogue0(QString getHost,QString getUserName);
};

#endif // LDAPCATALOGUE_H
