/* 
 * File:   TCP_FeW.h
 * Author: leer
 *
 * Created on June 28, 2011, 11:48 AM
 */

#ifndef _TCP_FEW_H
#define	_TCP_FEW_H

#include "TCPTahoe.h"
#include "IContextContainerFactory.h"
#include "TCPFewIContextContainerFactory.h"
#include "contexts/TCPFewIContextContainer.h"

class TCP_FeW : public TCPTahoe {
    
protected:
    TCP_FeW(int protocol = TCP_FEW, IContextContainerFactory* factory = new TCPFewIContextContainerFactory());
    
public:
    static TCP_FeW& instance();
    virtual ~TCP_FeW();

    //TCP Tahoe overrides?
    
};

#endif	/* _TCP_FEW_H */

