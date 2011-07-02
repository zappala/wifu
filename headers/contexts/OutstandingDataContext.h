/* 
 * File:   OutstandingDataContext.h
 * Author: rbuck
 *
 * Created on May 23, 2011, 3:57 PM
 */

#ifndef OutstandingDataContext_H
#define	OutstandingDataContext_H

#include "Context.h"
#include "Math.h"
#include <sys/types.h>

/**
 * This class is to be used by window based protocols such as TCP.
 * It keeps track of the following variables: ISS, SND_UNA, SND_NXT, and SND_MAX.
 * For details on these variables see RFC 793.
 * SND_MAX is the largest SND_NXT we have ever seen.
 *
 * Using these variables it is easy to compute how much data is outstanding.
 */
class OutstandingDataContext : public Context {
public:

    /**
     * Constructor.
     * Stores iss to use in a SYN packet.
     * Initializes SND_UNA, SND_NXT, and SND_MAX to zero.
     * @param iss Initial send sequence number.
     */
    OutstandingDataContext(u_int32_t iss);

    /**
     * Destructor.
     */
    virtual ~OutstandingDataContext();

    /**
     * @return The initial send sequence number.
     */
    u_int32_t get_iss() const;

    /**
     * Saves the initial send sequence number.
     * @param iss The initial send sequence number to save.
     */
    void set_iss(u_int32_t iss);

    /**
     * @return SND_NXT, defined as the sequence number of next byte to send.
     */
    u_int32_t get_snd_nxt() const;

    /**
     * Saves snd_nxt.
     * Also sets the maximum snd_nxt seen (saves SND_MAX).
     * @param snd_nxt The sequence number of next byte to send.
     */
    void set_snd_nxt(u_int32_t snd_nxt);

    /**
     * @return SND_UNA, defined as the sequence number of first unacknowledged byte.
     */
    u_int32_t get_snd_una() const;

    /**
     * Saves snd_una.
     * @param snd_nxt The sequence number of the first unacknowledged byte.
     */
    void set_snd_una(u_int32_t snd_una);

    /**
     * Computes and returns the number of outstanding bytes (SND.NXT - SND.UNA).
     * @return The number of outstanding bytes (SND.NXT - SND.UNA).
     */
    u_int32_t get_num_outstanding() const;

    /**
     * @return The maximum sequence number ever passed into set_snd_nxt().
     */
    u_int32_t get_snd_max() const;
    
private:

    /**
     * Initial send sequence number.
     */
    u_int32_t iss_;

    /**
     * Sequence number of the next byte to send.
     */
    u_int32_t snd_nxt_;

    /**
     * Sequence number of the first unacknowledged byte.
     */
    u_int32_t snd_una_;

    /**
     * Largest sequence number ever seen.
     */
    u_int32_t snd_max_;
};

#endif	/* OutstandingDataContext_H */

