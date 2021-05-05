/*
 * Copyright (C) 2015-2020 "IoT.bzh"
 * Author José Bollo <jose.bollo@iot.bzh>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

#define AFB_BINDING_VERSION 3
#include <afb/afb-binding.h>

//ping
static void ping(afb_req_t req)
{
	static int ping_count = 0;

	afb_req_success_f(req, json_object_new_int(ping_count), "Ping count = %d", ping_count);
	AFB_API_NOTICE(afbBindingV3root, "Verbosity macro at level notice invoked at ping invocation count = %d", ping_count);
	ping_count++;
}


//connection
static void connection(afb_req_t req)
{
	AFB_INFO("method 'connection' called!\n");

	//replies
	afb_req_success(req, NULL, NULL);
}

static const struct afb_auth _afb_auths_v2_monitor[] = {
	{.type = afb_auth_Permission, .text = "urn:AGL:permission:monitor:public:set"},
	{.type = afb_auth_Permission, .text = "urn:AGL:permission:monitor:public:get"},
	{.type = afb_auth_Or, .first = &_afb_auths_v2_monitor[1], .next = &_afb_auths_v2_monitor[0]}
};

/*
 * array of the verbs exported to afb-daemon
 */
static const afb_verb_t verbs[] = {
   { .verb="connection", .session= AFB_SESSION_NONE,  .callback=connection, .info= "Connection test" },
   { .verb="ping", .session=AFB_SESSION_NONE, .callback=ping, .info="ping test"},
   { .verb=NULL }
};


/*
 * description of the binding for afb-daemon
 */
const afb_binding_t afbBindingV3 = {
	//.api = "self_qml",
	.api = "helloworld",
	.specification = NULL,
	.verbs = verbs,
	.noconcurrency = 1
};


