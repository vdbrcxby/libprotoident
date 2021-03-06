/*
 *
 * Copyright (c) 2011-2016 The University of Waikato, Hamilton, New Zealand.
 * All rights reserved.
 *
 * This file is part of libprotoident.
 *
 * This code has been developed by the University of Waikato WAND
 * research group. For further information please see http://www.wand.net.nz/
 *
 * libprotoident is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * libprotoident is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */

#include <string.h>

#include "libprotoident.h"
#include "proto_manager.h"
#include "proto_common.h"

/* First two bytes definitely look like a little-endian length
 * field, so we could use that to match more reply types */

/* Port 9991, 9992 and 9993 */

static inline bool match_bdo_request(uint32_t payload, uint32_t len) {

        if (len == 111 && MATCH(payload, 0x6f, 0x00, 0x01, 0x9d))
                return true;
        if (len == 111 && MATCH(payload, 0x6f, 0x00, 0x01, 0x46))
                return true;
        if (len == 277 && MATCH(payload, 0x15, 0x01, 0x01, ANY))
                return true;
        if (len == 433 && MATCH(payload, 0xb1, 0x01, 0x01, ANY))
                return true;
        return false;

}

static inline bool match_bdo_reply(uint32_t payload, uint32_t len) {

        if (len == 112 && MATCH(payload, 0x70, 0x00, 0x01, ANY))
                return true;
        if (len == 113 && MATCH(payload, 0x71, 0x00, 0x01, ANY))
                return true;
        if (len == 119 && MATCH(payload, 0x77, 0x00, 0x01, 0x46))
                return true;

        return false;

}

static inline bool match_blackdesert(lpi_data_t *data, lpi_module_t *mod UNUSED) {


        if (match_bdo_request(data->payload[0], data->payload_len[0])) {
                if (match_bdo_reply(data->payload[1], data->payload_len[1]))
                        return true;
        }


        if (match_bdo_request(data->payload[1], data->payload_len[1])) {
                if (match_bdo_reply(data->payload[0], data->payload_len[0]))
                        return true;
        }

	return false;
}

static lpi_module_t lpi_blackdesert = {
	LPI_PROTO_BLACKDESERT,
	LPI_CATEGORY_GAMING,
	"BlackDesertOnline",
	12,
	match_blackdesert
};

void register_blackdesert(LPIModuleMap *mod_map) {
	register_protocol(&lpi_blackdesert, mod_map);
}

