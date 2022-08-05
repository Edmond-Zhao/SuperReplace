/* Created by Edmond  08/05/2022 */

#ifndef _TEST_H_
#define _TEST_H_

/* ---------- defined for test BEGIN ---------- */
char s_file_path_list[][ 256 ] = {
    "shared/proto/802.11ah.h",
    "shared/proto/802.11e.h",
    "shared/proto/802.11r.h",
    "shared/proto/802.1d.h",
    "shared/proto/802.3.h",
    "shared/proto/bcmarp.h",
    "shared/proto/bcmdhcp.h",
    "shared/proto/bcmeth.h",
    "shared/proto/bcmevent.h",
    "shared/proto/bcmicmp.h",
    "shared/proto/bcmip.h",
    "shared/proto/bcmipv6.h",
    "shared/proto/bcmproto.h",
    "shared/proto/bcmtcp.h",
    "shared/proto/bcmudp.h",
    "shared/proto/eap.h",
    "shared/proto/eapol.h",
    "shared/proto/ethernet.h",
    "shared/proto/event_log_set.h",
    "shared/proto/event_log_tag.h",
    "shared/proto/gspi.h",
    "shared/proto/nan.h",
    "shared/proto/p2p.h",
    "shared/proto/sdspi.h",
    "shared/proto/vlan.h",
    "shared/proto/wpa.h",
    "shared/proto/wps.h"
};


int gi_file_path_count = sizeof( s_file_path_list ) / 256;

char *s_target_pgh[] = {
    "<<<<<<< HEAD",
    " * Copyright (C) 2021, Broadcom. All Rights Reserved.",
    "=======",
    " * Copyright (C) 2022, Broadcom. All Rights Reserved.",
    ">>>>>>> broadcom_vendor_9.5.0h3",
    NULL
};

char *s_replace_with[] = {
    " * Copyright (C) 2022, Broadcom. All Rights Reserved.",
    NULL
};

/* ---------- defined for test END ---------- */

#endif /* _TEST_H_ */

