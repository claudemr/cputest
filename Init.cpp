/*
 *                      Yeppp! library implementation
 *
 * This file is part of Yeppp! library and licensed under the New BSD license.
 * See LICENSE.txt for the full text of the license.
 */

#include "yepPredefines.h"
#include "yepTypes.h"
#include "yepPrivate.h"
#include "yepLibrary.h"

#include "functions.h"

YepStatus YEPABI yepLibrary_Init() {
	YepStatus status = _yepLibrary_InitCpuInfo();
	if YEP_UNLIKELY(status != YepStatusOk) {
		return status;
	}

	return YepStatusOk;
}

FunctionPointer _yepLibrary_InitFunction(const FunctionDescriptor<YepStatus (*)()>* dispatchTable) {
	typedef FunctionDescriptor<YepStatus (*)()> GeneralizedFunctionDescriptor;
	const Yep64u unsupportedIsaFeatures = ~_isaFeatures;
	const Yep64u unsupportedSimdFeatures = ~_simdFeatures;
	const Yep64u unsupportedSystemFeatures = ~_systemFeatures;
	const GeneralizedFunctionDescriptor* startFunction = dispatchTable;
	const GeneralizedFunctionDescriptor* endFunction = startFunction;

	YepBoolean defaultImplementation;
	do {
		const YepBoolean defaultMicroarchitecture = (endFunction->microarchitecture == YepCpuMicroarchitectureUnknown);
		const YepBoolean defaultIsaFeatures = (endFunction->isaFeatures == YepIsaFeaturesDefault);
		const YepBoolean defaultSimdFeatures = (endFunction->simdFeatures == YepSimdFeaturesDefault);
		const YepBoolean defaultSystemFeatures = (endFunction->systemFeatures == YepSystemFeaturesDefault);
		defaultImplementation = defaultMicroarchitecture && defaultIsaFeatures && defaultSimdFeatures && defaultSystemFeatures;
		endFunction += 1;
	} while (!defaultImplementation);
	
	for (const YepCpuMicroarchitecture *targetMicroarchitecturePointer = _dispatchList; ; targetMicroarchitecturePointer++) {
		const YepCpuMicroarchitecture targetMicroarchitecture = *targetMicroarchitecturePointer;
		for (const GeneralizedFunctionDescriptor* currentFunction = startFunction; currentFunction != endFunction; currentFunction++) {
			if YEP_UNLIKELY(currentFunction->microarchitecture == targetMicroarchitecture) {
				const YepBoolean hasSupportedIsaFeatures = ((currentFunction->isaFeatures & unsupportedIsaFeatures) == 0);
				const YepBoolean hasSupportedSimdFeatures = ((currentFunction->simdFeatures & unsupportedSimdFeatures) == 0);
				const YepBoolean hasSupportedSystemFeatures = ((currentFunction->systemFeatures & unsupportedSystemFeatures) == 0);
				const YepBoolean hasSupportedFeatures = hasSupportedIsaFeatures && hasSupportedSimdFeatures && hasSupportedSystemFeatures;
				if (hasSupportedFeatures) {
					return currentFunction->function;
				}
			}
		}
	}
}

YepStatus YEPABI yepLibrary_Release() {
	return YepStatusOk;
}



#if defined(YEP_LINUX_OS)
	#if defined(YEP_ARM_CPU)
		asm (
			".section .version, \"S\",%progbits\n"
			".string \"0.0.0.0\"\n"
		);
	#else
		asm (
			".section .version, \"S\",@progbits\n"
			".string \"0.0.0.0\"\n"
		);
	#endif
#endif
