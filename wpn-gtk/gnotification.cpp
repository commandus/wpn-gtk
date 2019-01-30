/*
 * Copyright (c) 2019 Andrei Ivanov andrei.i.ivanov@commandus.com
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "gnotification.h"

/**
 * 
 */
static void notifyC2Cpp
(
	NotifyMessage *dst,
	const NotifyMessageC *src
)
{
	if (!(dst && src))
		return;
	dst->authorizedEntity = src->authorizedEntity;
	dst->title = src->title;
	dst->body = src->body;
	dst->icon = src->icon;
	dst->sound = src->sound;
	dst->link = src->link;
	dst->linkType = src->linkType;

	dst->urgency = src->urgency;
	dst->timeout = src->timeout;
	
	dst->category = src->category;
	dst->extra = src->extra;
	dst->data = src->data;
}

GtkNotification::GtkNotification()
	: persistent_id(), from(), appName(), appId(), sent(0)
{
}

GtkNotification::GtkNotification(
	const char *apersistent_id,
	const char *afrom,
	const char *aappName,
	const char *aappId,
	int64_t asent,
	NotifyMessageC *amsg
)
	: persistent_id(apersistent_id), from(afrom), appName(aappName), appId(aappId), sent(asent)
{
	notifyC2Cpp(&msg, amsg);
}

