#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SECURITY_H_
#define SECURITY_H_

	bool init();
	bool isAnyPortOpen();
	bool isAntiVirusRunning();

#endif

#ifdef __cplusplus
}
#endif