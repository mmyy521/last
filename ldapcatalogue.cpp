#include "ldapcatalogue.h"
//连接后获取目录树，并建立树结构，被谁调用->organizationtree
LDAPCatalogue::LDAPCatalogue()
{
}
//BaseTreeNode* LDAPCatalogue::GetCatalogue(QString getHost,QString getUserName)
//{
//    //qDebug()<<"host:"<<getHost;   //这里的问题，host没有信息？
//    LDAPMessage *res=NULL;//用于保存从服务器取到的所有实体
//    //主机LDAP信息
//    PWSTR host=(PWSTR)getHost.toStdWString().c_str();
//    ULONG port = LDAP_PORT;  // 端口

//    //初始化LDAP
//    LDAP *ld=NULL;
//    ld=ldap_init(host,port);

//    ULONG rc=0;
//    //从用户名中提取base
//    //base只能从用户名提取吗？用户名里面有Manager，base里面能有吗
//    QStringList baseList = getUserName.split(",");
//    QString baseStr = baseList[baseList.length()-2]+","+baseList[baseList.length()-1];

////    QString baseStr;
////    //QString baseStr = baseList[baseList.length()-1]; //这是？从倒数第1个查的
////    if(baseList[0] == "cn=admin")       //这里可能不够完善，如果是登陆时用的就从根部建树
////        baseStr = baseList[baseList.length()-1];
////    else                                //否则的话就以传进来的名称开始建，用于逐个删除
////        baseStr = getUserName;
//    //qDebug()<<"baseStr:"<< baseStr;  //dc=nodomain,从最根部查询，如果不是最根部可以吗？
//    //QString baseStr="ou=Graduate,o=xxxy,dc=nodomain";
//    //这里必须是树根的dn才管用，已经连接的状态，只要查询这个dn都能查到
//    //QString baseStr="dc=yz,dc=bjfu"; // /////?????
//    PWSTR base=(PWSTR)baseStr.toStdWString().c_str();
//    PWSTR filter = (PWSTR)L"(objectClass=*)";
//    PWCHAR attrs[4];
//    attrs[0] = (PWCHAR)L"cn";
//    attrs[1] = (PWCHAR)L"objectClass";
//    attrs[2] = (PWCHAR)L"ou";
//    attrs[3] = NULL;
//    rc=ldap_search_s(ld,base,LDAP_SCOPE_SUBTREE,filter,attrs,(ULONG)0,&res);
//    if(rc!=LDAP_SUCCESS){
//        qDebug("ldap_search all fail,rc:%d",rc);
//    }
//    else {
//        qDebug()<<"ldap_search success!";  //在这里
//    }
//    ULONG count = ldap_count_entries(ld, res); //为什么只有一个
//    qDebug()<<"count:"<<count;
//    //准备遍历所有实体
//    LDAPMessage *entry=NULL;
//    PWCHAR entryDN=NULL;
//    ULONG i=0;
//    const char * msg;
//    BerElement*ber=NULL;
//    BaseTreeNode *root;
//    BaseTreeNode *child;
//    int offset=0;//之前设置offset的初衷是原来的根是1个，如果大于1个的话就代表在删除节点，故设此
////    if(baseStr=="dc=yz,dc=bjfu")
////        offset=0;
////    else
////        offset=1;
//    for(i=0;i<count;i++)
//    {
//        QString dn;
//        //建立根节点
//        if(i==0){
//            entry=ldap_first_entry(ld,res);
//            entryDN=ldap_get_dn(ld,entry);
//            dn= QString::fromWCharArray(entryDN);
//            qDebug()<<"dn"<<dn;
//            //QStringList catalogueTree=dn.split(","); //第一个entry可能是只有一项，也可能有多项，所以建完根可能需要加孩子，也可能不需要
//            //offset = catalogueTree.length()-1;  //这里标志着如果根是两个以上组成，就是第一种情况
//            //if(offset)
//            root = new BaseTreeNode(dn);
//            //else
//            //root = new BaseTreeNode(catalogueTree[0]);
//            /*qDebug()<<"1:"<<catalogueTree[1];
//            child=new BaseTreeNode(catalogueTree[0]);
//            qDebug()<<"0:"<<catalogueTree[0];
//            root->addChildNode(child);*/
//        }
//        else
//        {
//            //提取名称形成树结构
//            //qDebug()<<"--------------------------------i:"<<i;
//            //root->traverse(root);
//            entry=ldap_next_entry(ld,entry);  //拿出下一个entry
//            entryDN=ldap_get_dn(ld,entry);      //从entry中获取dn
//            dn = QString::fromWCharArray(entryDN);  //将dn转换成String
//            qDebug()<<"dn:"<<dn;
//            //   ！！如果不是从根开始的话，会导致这一条：以,分开之后就和根节点对不上了，所以会找不到上一个节点
//            //   ！！因为是从中间开始，所以找不到头上的根，除非？把头上的根给掠过去

//            // !!每个节点都存全称可行吗，更复杂了，首先需要找到上一个节点（就是把当前节点的字符串去掉第一个,之前的内容）
//            // 然后存当前节点的全称
//            // !!因为查询出来的条目列表顺序是按照先小后大排列，所以其实不找上一个也能确定父节点是在的。

//            QStringList catalogueTree=dn.split(",");  //以 ， 分开而获得一个列表
////            for(int k=0;k<catalogueTree.length();k++)
////            {
////                qDebug()<<catalogueTree[k];
////            }
//            //ou=dep_network_engineering,ou=teacher,o=xxxy,dc=nodomain
//            //因为有根了，所以从xxxy开始判断，依次向前判断，如果树里没有找到就加进去，这个判断对重名的情况就会失效
//            //需要修改成递归的方式增加节点。
////            if(offset)
////            {
////                QString result;
////                int commaIndex = dn.indexOf(',');
////                if (commaIndex != -1) {
////                    result = dn.mid(commaIndex + 1);//这是找到待添加节点的父节点的名称
////                    qDebug() << result;
////                }
////                //找父节点在不在
////                BaseTreeNode* last_child = root->findNode(root,result);
////                BaseTreeNode* child = root->findNode(root,dn);
////                if((child==nullptr&&last_child!=nullptr)||(child!=nullptr&&child->getParentNode()->getName()!=result))
////                {

////                    //qDebug()<<"添加新的：子节点:"<<catalogueTree[j]<<"父节点："<<catalogueTree[j+1];
////                    BaseTreeNode* newChild;
////                    newChild=new BaseTreeNode(dn);
////                    BaseTreeNode *parent=last_child;   //上一个节点作为父节点
////                    parent->addChildNode(newChild);
////                }
////            }
//            //else
//            //如果是这样的话肯定查不到根节点，所以根节点只能一个，分开来，这也太一次性了。。。换个服务器怎么办。
//            //删除的必须另起一个函数
//            //这里减3是因为要从倒数第三个开始
////            for(int j=catalogueTree.length()-3;j>=0;j--)  //这里需要改，给树增加孩子有问题
////            {
//               //-!!!!!!!其实不用定位，根据查询到的条目规律，每次都是只有第一个需要添加到树里，只需要根据list找到第二个节点的位置即可
//                BaseTreeNode* last_child;  //定义一个孩子
//                last_child=root->findNode(root,catalogueTree,catalogueTree.length()-3);
//                qDebug()<<"----------------------------parent:"<<last_child->getName();
//                BaseTreeNode* newChild;
//                newChild=new BaseTreeNode(catalogueTree[0]);
//                //找到父亲节点，这样的话需要递归添加吧
//                //递归添加节点
//                //BaseTreeNode *parent=last_child;   //上一个节点作为父节点
//                last_child->addChildNode(newChild);
//                    //QString last_child_str=catalogueTree[j+1];  //最后一个孩子是第j+1个
//                    //qDebug()<<"catalogueTree[j]:"<<catalogueTree[j];
//                    //每次都从根节点开始，不对，不判断的话每次都会新建所有的路径，怎样知道是已经建立的还是需要新建
//                    //需要写一个函数，判断从根到当前的所有节点是否都已经存在，如果已经存在就不需要重新建立，不存在就重新建立。
//                    last_child=root->findNode(root,catalogueTree,catalogueTree.length()-3);
//                    //如果现在这个节点不存在，并且能找到其上一个节点，或者当前这个节点已经存在，但是当前的节点的父节点不是上一个节点，就添加这个节点
//    //              加重复了的原因？在于find函数
//                    //现在是要干什么：无论怎样都添加新节点，无需判断
////                    if((child==nullptr&&last_child!=nullptr)||(child!=nullptr&&child->getParentNode()->getName()!=last_child_str))
////                    {
//                        //qDebug()<<"添加新的：子节点:"<<catalogueTree[j]<<"父节点："<<catalogueTree[j+1];
////                        BaseTreeNode* newChild;
////                        newChild=new BaseTreeNode(catalogueTree[j]);
////                        //找到父亲节点，这样的话需要递归添加吧
////                        //递归添加节点
////                        BaseTreeNode *parent=last_child;   //上一个节点作为父节点
////                        parent->addChildNode(newChild);
////                    }
////                }
//            }
//        }
//        msg=(!i?"ldap_first_entry":"ldap_next_entry");
//        if(entry==NULL)
//        {
//            qDebug()<<"0 entry";
//        }
//        if( ber != NULL )
//            //ber_free(ber,0);
//        ber = NULL;

//    ldap_unbind_s(ld);
//    return root;
//}

BaseTreeNode* LDAPCatalogue::GetCatalogue(QString getHost,QString getUserName)
{
    LDAPMessage *res=NULL;//用于保存从服务器取到的所有实体
    //主机LDAP信息
    PWSTR host=(PWSTR)getHost.toStdWString().c_str();
    ULONG port = LDAP_PORT;  // 端口
    //初始化LDAP
    LDAP *ld=NULL;
    ld=ldap_init(host,port);
    ULONG rc=0,rc_detail=0;
    //从用户名中提取base
    QStringList baseList = getUserName.split(",");
    QString baseStr = baseList[baseList.length()-2]+","+baseList[baseList.length()-1];
    PWSTR base=(PWSTR)baseStr.toStdWString().c_str();
    //过滤条件
    PWSTR filter = (PWSTR)L"(objectClass=*)";
    PWCHAR attrs[4];
    attrs[0] = (PWCHAR)L"cn";
    attrs[1] = (PWCHAR)L"objectClass";
    attrs[2] = (PWCHAR)L"ou";
    attrs[3] = NULL;
    //查询
    //rc_detail = ldap_search_s(ld,base,LDAP_SCOPE_BASE,filter,attrs,(ULONG)0,&res);
    //结果包含？

    //查询子树
    rc = ldap_search_s(ld,base,LDAP_SCOPE_SUBTREE,filter,attrs,(ULONG)0,&res);
    if(rc!=LDAP_SUCCESS){
        qDebug("ldap_search all fail,rc:%d",rc);
    }
    else {
        qDebug()<<"ldap_search success!";  //在这里
    }
    ULONG count = ldap_count_entries(ld, res);
    //qDebug()<<"count:"<<count;

    LDAPMessage *entry=NULL;
    PWCHAR entryDN=NULL;
    ULONG i=0;
    const char * msg;
    BerElement *ber=NULL;
    BaseTreeNode *root;
    BaseTreeNode *child;
    for(i=0;i<count;i++)
    {
        QString dn;
        //建立根节点
        if(i==0){
            entry = ldap_first_entry(ld,res);
            entryDN = ldap_get_dn(ld,entry);
            dn= QString::fromWCharArray(entryDN);
            root = new BaseTreeNode(dn);
        }
        else
        {
            //提取名称形成树结构
            entry=ldap_next_entry(ld,entry);  //拿出下一个entry
            entryDN=ldap_get_dn(ld,entry);      //从entry中获取dn
            dn = QString::fromWCharArray(entryDN);  //将dn转换成String
            //qDebug()<<"dn:"<<dn;
            QStringList catalogueTree=dn.split(",");  //以,分开而获得一个列表
            BaseTreeNode* last_child;  //定义一个孩子
            last_child=root->findNode(root,catalogueTree,catalogueTree.length()-3);
            //qDebug()<<"----------------------------parent:"<<last_child->getName();
            BaseTreeNode* newChild;
            //newChild=new BaseTreeNode(catalogueTree[0]); //建树时，存的不是整个dn,而是
            newChild=new BaseTreeNode(dn);
            last_child->addChildNode(newChild);
            }
        }
        msg=(!i?"ldap_first_entry":"ldap_next_entry");
        if(entry==NULL)
        {
            qDebug()<<"0 entry";
        }
        if( ber != NULL )
            //ber_free(ber,0);
        ber = NULL;

    ldap_unbind_s(ld);
    return root;
}

BaseTreeNode* LDAPCatalogue::GetCatalogue0(QString getHost,QString getUserName)
{
    LDAPMessage *res=NULL;//用于保存从服务器取到的所有实体
    //主机LDAP信息
    PWSTR host=(PWSTR)getHost.toStdWString().c_str();
    ULONG port = LDAP_PORT;  // 端口
    //初始化LDAP
    LDAP *ld=NULL;
    ld=ldap_init(host,port);
    ULONG rc=0,rc_detail=0;
    //从用户名中提取base
    QStringList baseList = getUserName.split(",");  //这个数组有几个，现在从第一个开始建树
//    QString baseStr = baseList[baseList.length()-2]+","+baseList[baseList.length()-1];
    int offset = baseList.length()+1;     //每次都偏移“数组的个数-1”个？
    PWSTR base=(PWSTR)getUserName.toStdWString().c_str();
    //过滤条件
    PWSTR filter = (PWSTR)L"(objectClass=*)";
    PWCHAR attrs[4];
    attrs[0] = (PWCHAR)L"cn";
    attrs[1] = (PWCHAR)L"objectClass";
    attrs[2] = (PWCHAR)L"ou";
    attrs[3] = NULL;
    //查询子树
    rc = ldap_search_s(ld,base,LDAP_SCOPE_SUBTREE,filter,attrs,(ULONG)0,&res);
    if(rc!=LDAP_SUCCESS){
        qDebug("ldap_search all fail,rc:%d",rc);
    }
    else {
        qDebug()<<"ldap_search success!";  //在这里
    }
    ULONG count = ldap_count_entries(ld, res);
    //qDebug()<<"count:"<<count;

    LDAPMessage *entry=NULL;
    PWCHAR entryDN=NULL;
    ULONG i=0;
    const char * msg;
    BerElement *ber=NULL;
    BaseTreeNode *root;
    BaseTreeNode *child;
    for(i=0;i<count;i++)
    {
        QString dn;
        //建立根节点
        if(i==0){
            entry = ldap_first_entry(ld,res);
            entryDN = ldap_get_dn(ld,entry);
            dn= QString::fromWCharArray(entryDN);
            //qDebug()<<"根节点："<<dn;
            root = new BaseTreeNode(dn);
        }
        else
        {
            //提取名称形成树结构
            entry=ldap_next_entry(ld,entry);  //拿出下一个entry
            entryDN=ldap_get_dn(ld,entry);      //从entry中获取dn
            dn = QString::fromWCharArray(entryDN);  //将dn转换成String
            //qDebug()<<"dn:"<<dn;
            QStringList catalogueTree=dn.split(",");  //以,分开而获得一个列表
            BaseTreeNode* last_child;  //定义一个孩子
            last_child=root->findNode(root,catalogueTree,catalogueTree.length()-offset);
            //qDebug()<<"----------------------------parent:"<<last_child->getName();
            BaseTreeNode* newChild;
            //newChild=new BaseTreeNode(catalogueTree[0]); //建树时，存的不是整个dn,而是
            newChild=new BaseTreeNode(dn);
            last_child->addChildNode(newChild);
            }
        }
        msg=(!i?"ldap_first_entry":"ldap_next_entry");
        if(entry==NULL)
        {
            qDebug()<<"0 entry";
        }
        if( ber != NULL )
            //ber_free(ber,0);
        ber = NULL;

    ldap_unbind_s(ld);
    return root;
}
