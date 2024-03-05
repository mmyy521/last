//具体的 用于登录、删除、修改的函数
#include "serverconnect.h"
#include<QDebug>
#include<QMessageBox>
#include<QInputDialog>
#include<QUuid>
#include<QDateTime>
#include <QThread>
#include "basetreenode.h"
#include "ldapcatalogue.h"
//连接LDAP服务器并用此连接句柄进行增删改查，增删改查的具体函数需要在此实现
ServerConnect::ServerConnect(QWidget* parent):
    QWidget(parent)
{
}

int ServerConnect::LDAPConnect(){       //被调用了
    if(dn.trimmed()=="" || cred.trimmed()=="" ||ipAddress.trimmed()=="")   //验证是否为空
    {
        return 0;
    }
    this->host=(PWSTR)this->ipAddress.toStdWString().c_str();//将 IP 地址转换为 PWSTR 类型
    //初始化LDAP
    ld=ldap_init(host,port);
    if(ld==NULL){
        qDebug()<<"ldap_init error";
        return 0;
    }
    else{
        qDebug()<<"ldap_init success";          //控制台输出信息
    }

    ULONG rc=0;
    //默认版本是2，所以要设置协议版本为3
    rc=ldap_set_option(ld,LDAP_OPT_PROTOCOL_VERSION,&version);
    if(rc!=LDAP_SUCCESS){
        qDebug()<<"ldap_set_option fail";
        return 0;
    }
    else {
        qDebug()<<"ldap_set_option success";
    }

    //连接LDAP服务器
    rc=ldap_connect(ld,NULL);
    if(rc!=LDAP_SUCCESS){
        qDebug("ldap_connect fail rc:%d",rc);
        return 0;
    }
    else{
        qDebug()<<"ldap_connect success";
    }

    PWCHAR user=(PWCHAR)dn.toStdWString().c_str();
    PWCHAR password=(PWCHAR)cred.toStdWString().c_str();
    //绑定账号
    rc=ldap_bind_s(ld,user,password,method);   //绑定这里的method是.h文件里的 简单认证模式
    if(rc!=LDAP_SUCCESS){
        qDebug("ldap_bind fail rc:%d",rc);
        return 0;
    }
    else{
        qDebug()<<"ldap_bind success";      //绑定成功
        return 1;
    }
    return 0;
}

void ServerConnect::LDAPDisconnect(){
    ldap_unbind_s(this->ld);
    this->ld=NULL;
    this->dn="";
    this->cred="";
}

//添加用户节点---OK
bool ServerConnect::addUserNode(QString path){
    //qDebug()<<"添加用户 path:"<<path;
    mDialog = new MultipleDialog;   //多模态对话框
    QStringList valueList;          //字符串列表
    QString sn;                     //记录下在homeDirectory加上
    int gidN;
    if(mDialog->exec()==QDialog::Accepted)
    {
        valueList=mDialog->getStrings();    //从对话框中提取出需要的信息
        gidN= mDialog->getgid();
    }
    else if(mDialog->close()==true)
    {
        return false;
    }
    //保存属性
    LDAPMod *NewEntry[11];
    //保存要添加的实体
    PWSTR entryDn;
    //qDebug()<<"LIST:"<<valueList.size();
    //for(int i=0;i<valueList.size();i++)

    for(int i=0;i<5;i++)
    {
        //qDebug()<<valueList.at(i);
        if(i==0)//添加cn(Common Name)
        {
            LDAPMod allName;
            QString cn = valueList.at(i);
            allName.mod_type=(PWSTR)L"cn";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(cn.utf16()), NULL};
            //强制类型转换成一个什么类型？                                             访问值的函数
            allName.mod_op=LDAP_MOD_ADD;
            allName.mod_values=(PWSTR*)values;
            NewEntry[i]=&allName;
        }
        else if(i==1)//Surname
        {
            //添加sn
            sn = valueList.at(i);
            LDAPMod allName;
            allName.mod_type=(PWSTR)L"sn";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(sn.utf16()), NULL};
            allName.mod_op=LDAP_MOD_ADD;
            allName.mod_values=(PWSTR*)values;
            NewEntry[i]=&allName;
        }
        else if(i==2)//uid
        {
            QString dn = "uid="+valueList.at(i)+","+path;//这里的path是传进来的参数，之前的路径

            qDebug()<<"dn:"<<dn;
            //新增条目dn
            entryDn=(PWSTR)dn.toStdWString().c_str();
            LDAPMod allName;
            allName.mod_type=(PWSTR)L"uid";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(valueList.at(i).utf16()), NULL};
            //QString value="uidtest";
            //const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(value.utf16()), NULL};
            allName.mod_op=LDAP_MOD_ADD;
            allName.mod_values=(PWSTR*)values;
            NewEntry[i]=&allName;
        }
        else if(i==3)//jid
        {
            LDAPMod allName;
            allName.mod_type=(PWSTR)L"jid";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(valueList.at(i).utf16()), NULL};
            //QString value="jidtest";
            //const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(value.utf16()), NULL};
            allName.mod_op=LDAP_MOD_ADD;
            allName.mod_values=(PWSTR*)values;
            NewEntry[i]=&allName;
        }
        else if(i==4)
        {
            //添加userPassword
            LDAPMod allName;
            allName.mod_type=(PWSTR)L"userPassword";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(valueList.at(i).utf16()), NULL};
            //QString value="pwd";
            //const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(value.utf16()), NULL};
            allName.mod_op=LDAP_MOD_ADD;
            allName.mod_values=(PWSTR*)values;
            NewEntry[i]=&allName;
        }
    }
//cn
//    LDAPMod cn;    //定义个
//    //QString dn = "cn="+valueList.at(i)+","+path;//这里的path是传进来的参数，之前的路径
//    QString mycn = "TESTADD";
//    //QString dn = "cn=TEST0,"+path;//这里的path是传进来的参数，之前的路径
//    //qDebug()<<"dn:"<<dn;
//    //新增条目dn
//    //entryDn=(PWSTR)dn.toStdWString().c_str();
//    //添加cn
//    cn.mod_type=(PWSTR)L"cn";
//    const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(mycn.utf16()), NULL};
//    //强制类型转换成一个什么类型？                                             访问值的函数
//    cn.mod_op=LDAP_MOD_ADD;
//    cn.mod_values=(PWSTR*)values;
////sn
//    QString mysn = "oko";
//    LDAPMod surname;
//    surname.mod_type=(PWSTR)L"sn";
//    const wchar_t *snvalues[] = {reinterpret_cast<const wchar_t *>(mysn.utf16()), NULL};
//    surname.mod_op=LDAP_MOD_ADD;
//    surname.mod_values=(PWSTR*)snvalues;
////uid
//    LDAPMod uid;
//    uid.mod_type=(PWSTR)L"uid";
//    //const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(valueList.at(i).utf16()), NULL};
//    QString value="uidtest";

//    QString dn = "cn=TESTADD,"+path;//这里的path是传进来的参数，之前的路径
//    qDebug()<<"dn:"<<dn;
//    entryDn=(PWSTR)dn.toStdWString().c_str();

//    const wchar_t *uidvalues[] = {reinterpret_cast<const wchar_t *>(value.utf16()), NULL};
//    uid.mod_op=LDAP_MOD_ADD;
//    uid.mod_values=(PWSTR*)uidvalues;

////jid
//    LDAPMod jid;
//    jid.mod_type=(PWSTR)L"jid";
//    QString myjidvalue="jidtest";
//    const wchar_t *jidvalues[] = {reinterpret_cast<const wchar_t *>(myjidvalue.utf16()), NULL};
//    jid.mod_op=LDAP_MOD_ADD;
//    jid.mod_values=(PWSTR*)jidvalues;
////pwd
//    LDAPMod pwd;
//    pwd.mod_type=(PWSTR)L"userPassword";
//    //const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(valueList.at(i).utf16()), NULL};
//    QString mypwdvalue="pwd";
//    const wchar_t *pwdvalues[] = {reinterpret_cast<const wchar_t *>(mypwdvalue.utf16()), NULL};
//    pwd.mod_op=LDAP_MOD_ADD;
//    pwd.mod_values=(PWSTR*)values;

//uidNumber
    QString myuidnum = "";
    myuidnum += "10022";
    QStringList pathList = path.split(","); //保存追溯信息
    int len = pathList.length();
    for(int i=len-3; i>=0; i--)
    {
        //qDebug()<<pathList[i];
        QString result = pathList[i].section('=', 1, 1);
        myuidnum+=result;
    }
    QString mytitle = myuidnum;
    //获取这一个子树下的节点数目，作后缀 ？？是否需要过滤只有cn的节点-----存疑
    LDAPMessage *res=NULL;//用于保存从服务器取到的所有实体
    ULONG rc=0;
    PWSTR base=(PWSTR)path.toStdWString().c_str();
    PWSTR filter = (PWSTR)L"(objectClass=*)";
    PWCHAR attrs[4];
    attrs[0] = (PWCHAR)L"cn";
    attrs[1] = (PWCHAR)L"objectClass";
    attrs[2] = NULL;
    //查询
    rc = ldap_search_s(ld,base,LDAP_SCOPE_SUBTREE,filter,attrs,(ULONG)0,&res);
    if(rc!=LDAP_SUCCESS){
        qDebug("ldap_search all fail,rc:%d",rc);
    }
    else {
        qDebug()<<"ldap_search success!";
    }
    ULONG count = ldap_count_entries(ld, res);
    //qDebug()<<"子树下有多少个："<<count;//是否包括根节点自己,是
    //这里是控制位数是2，可能增加位数
    if(count<10)
    {
        myuidnum+="0";
    }
    myuidnum+="%1";
    QString uidnumber = myuidnum.arg(count);
    //qDebug()<<"uidnumber"<<uidnumber;
    LDAPMod uidNumber;
    uidNumber.mod_type=(PWSTR)L"uidNumber";
    const wchar_t *uidValue[]={reinterpret_cast<const wchar_t *>(uidnumber.utf16()),NULL};
    uidNumber.mod_op=LDAP_MOD_ADD;
    uidNumber.mod_values=(PWSTR*)uidValue;

//title
    LDAPMod title;
    title.mod_type=(PWSTR)L"title";
    const wchar_t *titlevalue[]={reinterpret_cast<const wchar_t *>(mytitle.utf16()),NULL};
    title.mod_op=LDAP_MOD_ADD;
    title.mod_values=(PWSTR*)titlevalue;

//gidNumber
    LDAPMod gidNumber;
    gidNumber.mod_type=(PWSTR)L"gidNumber";
//    wchar_t gidNumbervalue[2];
//    swprintf(gidNumbervalue, 2, L"%d", gidN);
//    qDebug()<<"gidNum:"<<gidN;
    QString mygidNumber = QString::number(gidN);    //在对话框中获取
//    QString mygidNumber = "200";
    const wchar_t *gidNumbervalue[]={reinterpret_cast<const wchar_t *>(mygidNumber.utf16()),NULL};
    gidNumber.mod_op=LDAP_MOD_ADD;
    gidNumber.mod_values=(PWSTR*)gidNumbervalue;

//homeDirectory
    QString myhomeDirectory="/home";
    for(int i=len-3; i>=0; i--)
    {
        myhomeDirectory+="/";
        //qDebug()<<pathList[i];
        QString result = pathList[i].section('=', 1, 1);
        myhomeDirectory+=result;
    }
    myhomeDirectory+="/";
    myhomeDirectory+=sn;
    //qDebug()<<"myhomeDirectory:"<<myhomeDirectory;
    LDAPMod homeDirectory;
    homeDirectory.mod_type=(PWSTR)L"homeDirectory";
    const wchar_t *homeDirectoryValue[]={reinterpret_cast<const wchar_t *>(myhomeDirectory.utf16()),NULL};
    homeDirectory.mod_op=LDAP_MOD_ADD;
    homeDirectory.mod_values=(PWSTR*)homeDirectoryValue;

    //添加类
    LDAPMod ObjectClass;
    ObjectClass.mod_type=(PWSTR)L"objectClass";
    const wchar_t *personValue[] = {L"posixAccount",L"JIDObject",L"organizationalPerson",L"person",L"inetOrgPerson",L"top",NULL};
    ObjectClass.mod_op=LDAP_MOD_ADD;
    ObjectClass.mod_values=(PWSTR*)personValue;

    NewEntry[5]=&uidNumber;
    NewEntry[6]=&title;
    NewEntry[7]=&gidNumber;
    NewEntry[8]=&homeDirectory;
    NewEntry[9]=&ObjectClass;
    NewEntry[10]=NULL;
    //不OK---和顺序有关系吗-没有关
//    NewEntry[0]=&cn;
//    NewEntry[1]=&surname;
//    NewEntry[2]=&uid;
//    NewEntry[3]=&jid;
//    NewEntry[5]=&pwd;


    //ULONG rc=0;//测试是否正常连接
//    QString baseStr = "ou=001,dc=bjfu,dc=edu.cn";
//    PWSTR base=(PWSTR)baseStr.toStdWString().c_str();
//    PWSTR filter = (PWSTR)L"(objectClass=*)";
//    LDAPMessage *res2=NULL;
//    PWCHAR attrs[4];
//    attrs[0] = (PWCHAR)L"cn";
//    attrs[1] = NULL;
//    rc = ldap_search_s(ld,base,LDAP_SCOPE_SUBTREE,filter,attrs,(ULONG)0,&res2);
//    if(rc!=LDAP_SUCCESS){
//        qDebug("ldap_search all fail,rc:%d",rc);
//    }
//    else {
//        qDebug()<<"ldap_search success again!";
//    }

    rc = ldap_add_s(ld, entryDn, NewEntry); //添加信息
    // NewEntry中每个属性都是LDAPmod类型的，添加条目需要两个参数，一个是dn,一个是各个属性的信息
     if (rc != LDAP_SUCCESS) {
         qDebug("ldap_add_s:rc %d",rc);
         QMessageBox::warning(NULL,"提示信息","添加失败");
         return false;
     }
     else{
         QMessageBox::information(NULL,"提示信息","添加成功");
         return true;
     }
}

//添加ou-------OK
bool ServerConnect::addOuNode(QString path){
    mOuDialog = new MulDialogOu;        //调用multipledialog
    QStringList list;
    if(mOuDialog->exec()==QDialog::Accepted)
    {
        list=mOuDialog->getStrings();
    }
    else if(mOuDialog->close()==true)
    {
        return false;
    }
    //保存属性   需要一个属性信息，一个dn信息
    LDAPMod *NewEntry[4];
    //保存要添加的实体
    PWSTR entryDn;
    LDAPMod allName,ou_description,ouClass;
    QString dn = "ou="+list.at(0)+","+path; //先生成dn
    qDebug()<<"dn:"<<dn;

    //新增条目dn
    entryDn=(PWSTR)dn.toStdWString().c_str();

    //添加ou
    allName.mod_type=(PWSTR)L"ou";
    const wchar_t *ouvalues[] = {reinterpret_cast<const wchar_t *>(list.at(0).utf16()), NULL};
    allName.mod_op=LDAP_MOD_ADD;
    allName.mod_values=(PWSTR*)ouvalues;
    NewEntry[0]=&allName;

    //添加description
    ou_description.mod_type=(PWSTR)L"description";
    const wchar_t *descriptionvalues[] = {reinterpret_cast<const wchar_t *>(list.at(1).utf16()), NULL};
    ou_description.mod_op=LDAP_MOD_ADD;
    ou_description.mod_values=(PWSTR*)descriptionvalues;
    NewEntry[1]=&ou_description;

    //添加organizationUnit
    ouClass.mod_type=(PWSTR)L"objectClass";
    const wchar_t *ouValues[] = {L"top",L"organizationalUnit", NULL};
    ouClass.mod_op=LDAP_MOD_ADD;
    ouClass.mod_values=(PWSTR*)ouValues;
    NewEntry[2]=&ouClass;
    //将最后一个也要为空
    NewEntry[3]=NULL;

    ULONG rc=0;
    rc = ldap_add_s(ld, entryDn, NewEntry);
     if (rc != LDAP_SUCCESS) {
         qDebug("ldap_add_s:rc %d",rc);
         QMessageBox::warning(NULL,"提示信息","添加失败");
         return false;
     }
     else{
         QMessageBox::information(NULL,"提示信息","添加成功");
         return true;
     }
}
//修改信息-----OK
bool ServerConnect::modifyNode(QString path){
    PWSTR entryDn=(PWSTR)path.toStdWString().c_str();
    ULONG rc=0;
    PWSTR filter = (PWSTR)L"(objectClass=*)"; //过滤条件
    PWCHAR attrs[18];
//    attrs[0] = (PWCHAR)L"cn";
//    attrs[1] = (PWCHAR)L"sn";
//    attrs[2] = (PWCHAR)L"description";
//    attrs[3] = (PWCHAR)L"title";
//    attrs[4] = (PWCHAR)L"uid";
//    attrs[5] = (PWCHAR)L"uidNumber";
//    attrs[6] = (PWCHAR)L"gidNumber";
//    attrs[7] = (PWCHAR)L"ou";
//    attrs[8]=NULL;
    attrs[0] = (PWCHAR)L"cn";
    attrs[1] = (PWCHAR)L"sn";
    attrs[2] = (PWCHAR)L"ou";
    attrs[3] = (PWCHAR)L"description";
    attrs[4] = (PWCHAR)L"uid";
    attrs[5] = (PWCHAR)L"uidNumber";
    attrs[6] = (PWCHAR)L"gidNumber";
    attrs[7] = (PWCHAR)L"homeDirectory";
    attrs[8] = (PWCHAR)L"title";
    attrs[9] = (PWCHAR)L"jid";
    attrs[10] = (PWCHAR)L"I";
    attrs[11] = (PWCHAR)L"host";
    attrs[12] = (PWCHAR)L"o";
    attrs[13] = (PWCHAR)L"initials";
    attrs[14] = (PWCHAR)L"labeledURI";
    attrs[15] = (PWCHAR)L"pager";
    attrs[16]=NULL;
    LDAPMessage *res=NULL;//用于保存从服务器取到的所有实体
    //查询
    rc=ldap_search_s(ld,entryDn,LDAP_SCOPE_BASE,filter,attrs,(ULONG)0,&res);
    if(rc!=LDAP_SUCCESS){
        QMessageBox::warning(NULL,"提示信息","修改失败");
        return false;
    }
    else {
        LDAPMessage *entry=NULL;
        BerElement*ber=NULL;
        PWCHAR attribute=NULL;
        PWCHAR *values=NULL;
        ULONG iValue=0;

        //获取实体
        entry=ldap_first_entry(ld,res);  //获取第一个实体信息
        attribute=ldap_first_attribute(ld,entry,&ber);//获取当前实体的属性
        QString str= QString::fromWCharArray(attribute);
        QStringList stringList;
        while(attribute!=NULL)
        {
            //获取属性值
            values=ldap_get_values(ld,entry,attribute);
            if(values==NULL)
            {
                qDebug()<<"没有属性值";
            }
            else
            {
                iValue=ldap_count_values(values);
                ULONG j;//用于for循环输出每个value
                for(j=0;j<iValue;j++)
                {
                    QString detailMessage="";
                    detailMessage=str+':';
                    QString str1= QString::fromWCharArray(values[j]);
                    detailMessage=detailMessage+str1;
                    stringList.append(detailMessage);
                }
            }
            if(values!=NULL)
            {
                ldap_value_free(values);
            }
            values=NULL;
            ldap_memfree(attribute);

            //下一个属性
            attribute=ldap_next_attribute(ld,entry,ber);
            str= QString::fromWCharArray(attribute);
        }
        modDialog = new ModifyDialog(nullptr,stringList);
        QStringList modList;
        //QString photo;
        if(modDialog->exec()==QDialog::Accepted)
        {
            modList=modDialog->getStrings();
            //photo = modDialog->getPicBuff();
        }
        else if(modDialog->close()==true)
        {
            return false;
        }

        QStringList modAttr;
        QStringList modValue;
        //qDebug()<<"modList.size()"<<modList.size();
        for(int i=0;i<modList.size();i++)
        {
            QStringList attr = stringList.at(i).split(":");
            qDebug()<<attr[0];
            qDebug()<<modList.at(i);
            modAttr.append(attr[0]);
            modValue.append(modList.at(i));
            if(QString::compare(attr[0],"cn")==0)
            {
                if(attr[1]!=modList.at(i))  //如果和之前不同就修改dn
                {
                    PWSTR oldDn = (PWSTR)path.toStdWString().c_str();
                    QStringList oldName = path.split(",");
                    attr[1] = "cn="+modList.at(i);
                    oldName[0] = attr[1];
                    QString newName = oldName.join(",");
                    PWSTR newDn = (PWSTR)newName.toStdWString().c_str();
                    qDebug()<<"new cn:"<<newName;
                    rc = ldap_modrdn_s(ld,oldDn,newDn);
                    if(rc!=LDAP_SUCCESS)
                    {
                        qDebug("ldap_add_s:rc %d",rc);
                        QMessageBox::warning(NULL,"提示信息","修改失败");
                        return false;
                    }
                    else
                    {
                        if(modList.size()==1)
                        {
                            QMessageBox::information(NULL,"提示信息","修改成功");
                            return true;
                        }
                        else
                        {
                            path = newName;
                        }
                    }
                }
            }
            else if(QString::compare(attr[0],"ou")==0)
            {
                if(attr[1]!=modList.at(i))
                {
                    PWSTR oldDn = (PWSTR)path.toStdWString().c_str();
                    QStringList oldName = path.split(",");
                    attr[1] = "ou="+modList.at(i);
                    oldName[0] = attr[1];
                    QString newName = oldName.join(",");
                    PWSTR newDn = (PWSTR)newName.toStdWString().c_str();
                    qDebug()<<newName;
                    rc = ldap_modrdn_s(ld,oldDn,newDn);
                    if(rc!=LDAP_SUCCESS)
                    {
                        qDebug("ldap_add_s:rc %d",rc);
                        QMessageBox::warning(NULL,"提示信息","修改失败");
                        return false;
                    }
                    else
                    {
                        if(modList.size()==1)
                        {
                            QMessageBox::information(NULL,"提示信息","修改成功");
                            return true;
                        }
                        else
                        {
                            path = newName;
                        }
                    }
                }
            }
            //这里出现问题----为啥啊
//            qDebug()<<attr[0];
//            qDebug()<<modList.at(i);
        }

        if(!modifyConnect(path,modAttr,modValue))  //将需要修改的属性和信息加进两个list之后，调用下面这个函数进行修改
            return false;
        return true;
    }
}
//修改信息--被上面修改调用
bool ServerConnect::modifyConnect(QString path,QStringList attrName,QStringList attrValue)
{
    qDebug()<<"modify,path:"<<path;
    qDebug()<<"attrName.size():"<<attrName.size();
    PWSTR entryDn = (PWSTR)path.toStdWString().c_str();
    //保存属性
    LDAPMod *NewEntry[attrName.size()+1];
    int count=0;
    for(int i=0;i<attrName.size();i++)
    {
        //修改属性
        QString attr = attrName.at(i);
        qDebug()<<attr;
        if(attr=="sn")
        {
            LDAPMod allName;
            allName.mod_type=(PWSTR)L"sn";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(attrValue.at(i).utf16()), NULL};
            allName.mod_op=LDAP_MOD_REPLACE;
            allName.mod_values=(PWSTR*)values;
            NewEntry[count]=&allName;
            count++;
        }
        else if(attr=="description")
        {
            LDAPMod allName;
            allName.mod_type=(PWSTR)L"description";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(attrValue.at(i).utf16()), NULL};
            allName.mod_op=LDAP_MOD_REPLACE;
            allName.mod_values=(PWSTR*)values;
            NewEntry[count]=&allName;
            count++;
        }
        else if(attr=="uid")
        {
            LDAPMod allName;
            allName.mod_type=(PWSTR)L"uid";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(attrValue.at(i).utf16()), NULL};
            allName.mod_op=LDAP_MOD_REPLACE;
            allName.mod_values=(PWSTR*)values;
            NewEntry[count]=&allName;
            count++;
        }
        else if(attr=="homeDirectory")
        {
            LDAPMod allName;
            allName.mod_type=(PWSTR)L"homeDirectory";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(attrValue.at(i).utf16()), NULL};
            allName.mod_op=LDAP_MOD_REPLACE;
            allName.mod_values=(PWSTR*)values;
            NewEntry[count]=&allName;
            count++;
        }
        else if(attr=="gidNumber")
        {
            LDAPMod allName;
            allName.mod_type=(PWSTR)L"gidNumber";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(attrValue.at(i).utf16()), NULL};
            allName.mod_op=LDAP_MOD_REPLACE;
            allName.mod_values=(PWSTR*)values;
            NewEntry[count]=&allName;
            count++;
        }
        else if(attr=="jid")
        {
            LDAPMod allName;
            allName.mod_type=(PWSTR)L"jid";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(attrValue.at(i).utf16()), NULL};
            allName.mod_op=LDAP_MOD_REPLACE;
            allName.mod_values=(PWSTR*)values;
            NewEntry[count]=&allName;
            count++;
        }
        else if(attr=="host")
        {
            LDAPMod allName;
            allName.mod_type=(PWSTR)L"host";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(attrValue.at(i).utf16()), NULL};
            allName.mod_op=LDAP_MOD_REPLACE;
            allName.mod_values=(PWSTR*)values;
            NewEntry[count]=&allName;
            count++;
        }
        else if(attr=="I")
        {
            LDAPMod allName;
            allName.mod_type=(PWSTR)L"I";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(attrValue.at(i).utf16()), NULL};
            allName.mod_op=LDAP_MOD_REPLACE;
            allName.mod_values=(PWSTR*)values;
            NewEntry[count]=&allName;
            count++;
        }
        else if(attr=="o")
        {
            LDAPMod allName;
            allName.mod_type=(PWSTR)L"o";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(attrValue.at(i).utf16()), NULL};
            allName.mod_op=LDAP_MOD_REPLACE;
            allName.mod_values=(PWSTR*)values;
            NewEntry[count]=&allName;
            count++;
        }
        else if(attr=="initials")
        {
            LDAPMod allName;
            allName.mod_type=(PWSTR)L"initials";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(attrValue.at(i).utf16()), NULL};
            allName.mod_op=LDAP_MOD_REPLACE;
            allName.mod_values=(PWSTR*)values;
            NewEntry[count]=&allName;
            count++;
        }
        else if(attr=="labeledURI")
        {
            LDAPMod allName;
            allName.mod_type=(PWSTR)L"labeledURI";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(attrValue.at(i).utf16()), NULL};
            allName.mod_op=LDAP_MOD_REPLACE;
            allName.mod_values=(PWSTR*)values;
            NewEntry[count]=&allName;
            count++;
        }
        else if(attr=="pager")
        {
            LDAPMod allName;
            allName.mod_type=(PWSTR)L"pager";
            const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(attrValue.at(i).utf16()), NULL};
            allName.mod_op=LDAP_MOD_REPLACE;
            allName.mod_values=(PWSTR*)values;
            NewEntry[count]=&allName;
            count++;
        }
    }
    //将最后一个也要为空
    NewEntry[count]=NULL;
    ULONG rc=0;
    rc = ldap_modify_s(ld, entryDn, NewEntry);
     if (rc != LDAP_SUCCESS) {
         qDebug("ldap_modify_s:rc %d",rc);
         QMessageBox::warning(NULL,"提示信息","修改失败");
         return false;
     }
     else{
         QMessageBox::information(NULL,"提示信息","修改成功");
         return true;
     }
}

//删除结点-----OK
bool ServerConnect::deleteNode(QString path){
    ULONG rc=0;
    qDebug()<<"删除条目及其子条目："<<path;
    LDAPCatalogue *lc = new LDAPCatalogue();  //重新定义的是否需要重新连接服务器？
    BaseTreeNode *curr_tree = lc->GetCatalogue0(ipAddress,path);  //这里返回的是根节点
    curr_tree->traverse(curr_tree);
    rc = my_ldap_delete_s(curr_tree);
    //rc = ldap_delete_s(ld, entryDn);  //-----xxx这个语句不能删除带子节点的节点，得递归删除
    if (rc != true) {
        qDebug("ldap_delete_s:rc %d",rc);
        QMessageBox::warning(NULL,"提示信息","删除失败");
        return false;
    }
    else{
         QMessageBox::information(NULL,"提示信息","删除成功");
         return true;
    }
}
//可以删啦！
bool ServerConnect::my_ldap_delete_s(BaseTreeNode *root)
{
    //首先找到这一个条目在树里的位置，然后把这个分支下面的节点的dn拿出来用delete删除
    //找到了之后得到名称然后还需要拼接上后面的名字才是全称
    if(!root)
        return true;
    for(int i=0;i<root->childs().length();i++)
    {
        my_ldap_delete_s(root->childs().at(i));
    }
    ULONG rc = ldap_delete_s(ld,(PWCHAR)(root->getName()).toStdWString().c_str());
    if (rc != LDAP_SUCCESS) {
        qDebug()<<"失败："<<root->getName();
        qDebug("ldap_delete_s:rc %d",rc);
        //QMessageBox::warning(NULL,"提示信息","删除失败");
        return false;
    }
    else{
        qDebug()<<"删除成功："<<root->getName();
    }
    return true;
}

//显示节点的详细信息---OK
void ServerConnect::detailNode(QString path,DetailWidget *deWidget){
    PWSTR entryDn=(PWSTR)path.toStdWString().c_str();
    ULONG rc=0;
    PWSTR filter = (PWSTR)L"(objectClass=*)";
    PWCHAR attrs[18];
    attrs[0] = (PWCHAR)L"cn";
    attrs[1] = (PWCHAR)L"sn";
    attrs[2] = (PWCHAR)L"ou";
    attrs[3] = (PWCHAR)L"description";
    attrs[4] = (PWCHAR)L"uid";
    attrs[5] = (PWCHAR)L"uidNumber";
    attrs[6] = (PWCHAR)L"gidNumber";
    attrs[7] = (PWCHAR)L"homeDirectory";
    attrs[8] = (PWCHAR)L"title";
    attrs[9] = (PWCHAR)L"jid";
    attrs[10] = (PWCHAR)L"l";
    attrs[11] = (PWCHAR)L"host";
    attrs[12] = (PWCHAR)L"o";
    attrs[13] = (PWCHAR)L"initials";
    attrs[14] = (PWCHAR)L"labeledURI";
    attrs[15] = (PWCHAR)L"pager";
    attrs[16]=NULL;
    LDAPMessage *res=NULL;//用于保存从服务器取到的所有实体
    qDebug()<<"查询条目详细信息";
    //QThread::msleep(3);
    rc=ldap_search_s(ld,entryDn,LDAP_SCOPE_BASE,filter,attrs,(ULONG)0,&res);
    if(rc!=LDAP_SUCCESS){
        QMessageBox::warning(NULL,"提示信息","查找失败");
    }
    else {
        qDebug()<<"查询成功!";
        LDAPMessage *entry=NULL;
        BerElement*ber=NULL;
        PWCHAR attribute=NULL;
        PWCHAR *values=NULL;
        ULONG iValue=0;
        //获取实体
        entry=ldap_first_entry(ld,res);
        //获取当前实体的属性
        attribute = ldap_first_attribute(ld,entry,&ber);
        QString str = QString::fromWCharArray(attribute);   //获取属性的名称
        //生成初始化信息的窗口
        while(attribute!=NULL)
        {
            //获取属性值
            values=ldap_get_values(ld, entry, attribute);
            if(values==NULL)
            {
                return;
            }
            else
            {
                iValue = ldap_count_values(values);
                //qDebug()<<"attribute count："<<int(iValue);  //一条属性中有几条。
                ULONG j;//用于for循环输出每个value
                for(j=0;j<iValue;j++)
                {
                    QString str1= QString::fromWCharArray(values[j]);
                    qDebug()<< str << ":" <<str1;
                    //设置详情窗口内容
                    if(str=="cn")
                    {
                        deWidget->cn=str1;
                    }
                    else if(str=="sn")
                    {
                        deWidget->sn=str1;
                    }
                    else if(str=="ou")
                    {
                        deWidget->ou=str1;
                    }
                    else if(str=="description")
                    {
                        deWidget->description=str1;
                    }
                    else if(str=="uid")
                    {
                        deWidget->uid=str1;
                    }
                    else if(str=="uidNumber")
                    {
                        deWidget->uidNumber=str1;
                    }
                    else if(str=="gidNumber")
                    {
                        deWidget->gidNumber=str1;
                    }
//                    else if(str=="member")
//                    {
//                        if(deWidget->member=="")
//                            deWidget->member=str1;
//                        else
//                            deWidget->member=deWidget->member.append("\n").append(str1);
//                    }
                    else if(str=="homeDirectory")
                    {
                        deWidget->homeDirectory=str1;
                    }
                    else if(str=="title")
                    {
                        deWidget->title=str1;
                    }
                    else if(str=="jid")
                    {
                        deWidget->jid=str1;
                    }
                    else if(str=="host")
                    {
                        deWidget->host=str1;
                    }
                    else if(str=="l")
                    {
                        deWidget->l=str1;
                    }
                    else if(str=="o")
                    {
                        deWidget->o=str1;
                    }
                    else if(str=="initials")
                    {
                        deWidget->initials=str1;
                    }
                    else if(str=="labeledURI")
                    {
                        deWidget->labeledURI=str1;
                    }
                    else if(str=="pager")
                    {
                        deWidget->pager=str1;
                    }
                }
            }
            if(values!=NULL)
            {
                ldap_value_free(values);
            }
            values=NULL;
            ldap_memfree(attribute);

            //下一个属性
            attribute=ldap_next_attribute(ld,entry,ber);
            str= QString::fromWCharArray(attribute);
        }
    }
}
//后做
void ServerConnect::modifyPwd(QString path)
{
    mPwd = new ModifyPwd(nullptr,false);  //调用modifypwd文件
    QString oldPwd;
    if(mPwd->exec()==QDialog::Accepted)
    {
        qDebug()<<"修改密码";
        oldPwd=mPwd->getString();
        if(oldPwd==this->cred)
        {
            qDebug()<<"密码一样";
            newPwd = new ModifyPwd(nullptr,true);
            if(newPwd->exec()==QDialog::Accepted)
            {
                QStringList pwdList=newPwd->getPwd();
                QString newPassword;
                QString confirmPassword;
                for(int i=0;i<pwdList.size();i++)
                {
                    if(i==0)
                        newPassword=pwdList.at(i);
                    else if(i==1)
                        confirmPassword=pwdList.at(i);
                }
                if(QString::compare(newPassword,confirmPassword)==0)
                {
                    qDebug()<<path;
                    PWSTR entryDn = (PWSTR)path.toStdWString().c_str();
                    //保存属性
                    LDAPMod *NewEntry[2];
                    LDAPMod entryNew;
                    entryNew.mod_type=(PWSTR)L"userPassword";
                    entryNew.mod_op=LDAP_MOD_REPLACE;
                    const wchar_t *values[] = {reinterpret_cast<const wchar_t *>(newPassword.utf16()), NULL};
                    entryNew.mod_values=(PWSTR*)values;
                    NewEntry[0]=&entryNew;
                    NewEntry[1]=NULL;

                    ULONG rc=0;
                    rc = ldap_modify_s(ld, entryDn, NewEntry);
                     if (rc != LDAP_SUCCESS) {
                         qDebug("ldap_modify_s:rc %d",rc);
                         QMessageBox::warning(NULL,"提示信息","修改密码失败");
                         //return false;
                     }
                     else{
                         QMessageBox::information(NULL,"提示信息","修改密码成功");
                         this->cred=newPassword;
                         //return true;
                     }
                }
                else
                {
                    QMessageBox::warning(NULL,"提示信息","两次密码不同");
                }
            }
            else{
                newPwd->close();
            }
        }
        else{
            QMessageBox::warning(NULL,"提示信息","密码错误");
        }
    }
    else{
        mPwd->close();
    }
}
//添加组节点----去掉
bool ServerConnect::addGroupNode(QString path,GroupDialog *groupWidget){
    groupWidget->ip=ipAddress;      //传进来的这个值是干嘛的
    groupWidget->path=path;
    groupWidget->show();
    QString name;
    QStringList checkItem;
    //获取名称以及选择的用户
    if(groupWidget->exec()==QDialog::Accepted)
    {
        checkItem = groupWidget->getTreeList();
        name = groupWidget->getName();
        if(name.trimmed()=="")//判断name是否为空
        //返回值为去除了开头和结尾的空白字符串，这里的空白指QChar::isSpace()返回值为true，
        //比如'\t','\n','\v','\f','\r'和' ';
        {
            qDebug()<<"name is blank";
            return false;
        }
        else{
            //保存属性
            LDAPMod *NewEntry[4];
            LDAPMod groupName,groupClass;
            QString dn = "cn="+name+","+path;   //添加的时候dn依然是用cn加名字
            qDebug()<<dn;
            //新增条目dn
            PWSTR entryDn=(PWSTR)dn.toStdWString().c_str();
            //添加cn
            groupName.mod_type=(PWSTR)L"cn";
            const wchar_t *nameValues[] = {reinterpret_cast<const wchar_t *>(name.utf16()), NULL};
            groupName.mod_op=LDAP_MOD_ADD;
            groupName.mod_values=(PWSTR*)nameValues;
            NewEntry[0]=&groupName;

            groupClass.mod_type=(PWSTR)L"objectClass";
            groupClass.mod_op=LDAP_MOD_ADD;
            const wchar_t *classValues[] = {L"groupOfNames", NULL};
            groupClass.mod_values=(PWSTR*)classValues;
            NewEntry[1]=&groupClass;

            LDAPMod member;
            member.mod_type=(PWSTR)L"member";
            const wchar_t *values[checkItem.size()+1];
            //添加member
            for(int i=0;i<checkItem.size();i++)
            {
                values[i]=reinterpret_cast<const wchar_t *>(checkItem.at(i).utf16());
            }
            values[checkItem.size()]=NULL;
            member.mod_op=LDAP_MOD_ADD;
            member.mod_values=(PWSTR*)values;
            NewEntry[2]=&member;

            //添加空白
            NewEntry[3]=NULL;
            ULONG rc=0;
            rc = ldap_add_s(ld, entryDn, NewEntry);
             if (rc != LDAP_SUCCESS) {
                 qDebug("ldap_add_s:rc %d",rc);
                 QMessageBox::warning(NULL,"提示信息","添加失败");
                 return false;
             }
             else{
                 for(int i=0;i<checkItem.size();i++)
                 {
                     QString nowName = checkItem.at(i);
                     qDebug()<<nowName;
                     //原本memberdn
                     PWSTR memberDn=(PWSTR)nowName.toStdWString().c_str();
                     //修改dn
                     QString newName = nowName.split(",")[0]+","+dn;
                     qDebug()<<newName;
                     PWSTR newDn = (PWSTR)newName.toStdWString().c_str();
                     rc = ldap_modrdn_s(ld,memberDn,newDn);
                     if(rc!=LDAP_SUCCESS)
                     {
                         qDebug("ldap_add_s:rc %d",rc);
                         QMessageBox::warning(NULL,"提示信息","添加失败");
                         return false;
                     }
                 }
                 QMessageBox::information(NULL,"提示信息","添加成功");
                 return true;
             }
        }
    }
    else{
        qDebug()<<"quit";
    }
}
