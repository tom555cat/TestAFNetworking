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

// 是否要验证证书中的CN字段，默认是YES
@property (nonatomic, assign) BOOL validatesDomainName;

///-----------------------------------------
/// @name 从Bundle中获取证书
///-----------------------------------------

// 当时用AFNetworking时，你必须使用这个方法去获取证书，来指定自己的安全策略
+ (NSSet <NSData *> *)certificatesInBundle:(NSBundle *)bundle;

// 返回共享的默认安全策略，不允许invalid证书，需要验证CN，不验证预埋证书或者预埋证书的公钥
+ (instancetype)defaultPolicy;

///---------------------
/// @name 初始化
///---------------------

// 创建并返回一个pinningMode的安全策略
+ (instancetype)policyWithPinningMode:(AFSSLPinningMode)pinningMode;

// 创建并返回一个安全策略，模式为pinningMode，并且指定了预埋证书数据
+ (instancetype)policyWithPinningMode:(AFSSLPinningMode)pinningMode withPinnedCertificates:(NSSet <NSData *> *)pinnedCertificates;

/**
 Whether or not the specified server trust should be accepted, based on the security policy.
 
 This method should be used when responding to an authentication challenge from a server.
 
 @param serverTrust The X.509 certificate trust of the server.
 @param domain The domain of serverTrust. If `nil`, the domain will not be validated.
 
 @return Whether or not to trust the server.
 */
// 该方法在响应服务器的认证质询的时候调用，serverTru
- (BOOL)evaluateServerTrust:(SecTrustRef)serverTrust
                  forDomain:(nullable NSString *)domain;

@end

NS_ASSUME_NONNULL_END
