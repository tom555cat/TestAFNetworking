//
//  AFSecurityPolicy.h
//  TestAFNetworking
//
//  Created by tongleiming on 2019/8/5.
//  Copyright © 2019 tongleiming. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, AFSSLPinningMode) {
    AFSSLPinningModeNone,
    AFSSLPinningModePublicKey,
    AFSSLPinningModeCertificate,
};

NS_ASSUME_NONNULL_BEGIN

@interface AFSecurityPolicy : NSObject

// 服务器证书验证标准，默认是AFSSLPinningModeNone，只使用系统的信任机构公钥来验证服务器证书
@property (readonly, nonatomic, assign) AFSSLPinningMode SSLPinningMode;

/**
 The certificates used to evaluate server trust according to the SSL pinning mode.
 
 By default, this property is set to any (`.cer`) certificates included in the target compiling AFNetworking. Note that if you are using AFNetworking as embedded framework, no certificates will be pinned by default. Use `certificatesInBundle` to load certificates from your target, and then create a new policy by calling `policyWithPinningMode:withPinnedCertificates`.
 
 Note that if pinning is enabled, `evaluateServerTrust:forDomain:` will return true if any pinned certificate matches.
 */
// 应该是本地保存的证书
@property (nonatomic, strong, nullable) NSSet <NSData *> *pinnedCertificates;

// 是否信任服务器证书，如果服务器证书是一个invalid或者过期的SSL证书，默认是NO。
#warning 看这个变量在判断流程中处于哪一环节
@property (nonatomic, assign) BOOL allowInvalidCertificates;

@property (nonatomic, assign) BOOL validatesDomainName;

@end

NS_ASSUME_NONNULL_END
