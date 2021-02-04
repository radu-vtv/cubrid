/*
 * Copyright 2008 Search Solution Corporation
 * Copyright 2021 CUBRID Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include "server_type.hpp"

#include "system_parameter.h"

static SERVER_TYPE server_type;

void init_server_type ()
{
  server_type = (SERVER_TYPE) prm_get_integer_value (PRM_ID_SERVER_TYPE);
}

SERVER_TYPE get_server_type ()
{
  return server_type;
}

void init_page_server_hosts (std::string hosts)
{
  assert (server_type == SERVER_TYPE_TRANSACTION);

  auto col_pos = hosts.find (":");
  if (col_pos == std::string::npos)
    {
      er_set (ER_WARNING_SEVERITY, ARG_FILE_LINE, ER_PSH_MISSING_SEPARATOR, 0);
      return;
    }

  if (col_pos < 1)
    {
      er_set (ER_WARNING_SEVERITY, ARG_FILE_LINE, ER_PSH_EMPTY_HOSTNAME, 0);
      return;
    }

  if (col_pos == hosts.length () - 1)
    {
      er_set (ER_WARNING_SEVERITY, ARG_FILE_LINE, ER_PSH_MISSING_PORT, 0);
      return;
    }

  long port = -1;
  try
    {
      port = std::stol (hosts.substr (col_pos+1));
    }
  catch (...)
    {
    }

  if (port < 1 || port > 65535)
    {
      er_set (ER_WARNING_SEVERITY, ARG_FILE_LINE, ER_PSH_INVALID_PORT, 0);
      return;
    }

  // host and port seem to be OK
  std::string host = hosts.substr (0, col_pos);
  er_log_debug (ARG_FILE_LINE, "Page server hosts: %s port: %d\n", host.c_str (), port);
}
