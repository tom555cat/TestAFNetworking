//
//  AFSecurityPolicy.m
//  TestAFNetworking
//
//  Created by tongleiming on 2019/8/5.
//  Copyright © 2019 tongleiming. All rights reserved.
//

#import "AFSecurityPolicy.h"

@interface AFSecurityPolicy ()

@property (readwrite, nonatomic, assign) AFSSLPinningMode SSLPinningMode;
// 预埋证书的公钥集合
@property (readwrite, nonatomic, strong) NSSet *pinnedPublicKeys;

@end

@implementation AFSecurityPolicy

+ (NSSet *)certificatesInBundle:(NSBundle *)bundle {
    NSArray *paths = [bundle pathsForResourcesOfType:@"cer" inDirectory:@"."];
    
    NSMutableSet *certificates = [NSMutableSet setWithCapacity:[paths count]];
    for (NSString *path in paths) {
        NSData *certificateData = [NSData dataWithContentsOfFile:path];
        [certificates addObject:certificateData];
    }
    
    return [NSSet setWithSet:certificates];
}

+ (instancetype)defaultPolicy {
    AFSecurityPolicy *securityPolicy = [[self alloc] init];
    securityPolicy.SSLPinningMode = AFSSLPinningModeNone;
    
    return securityPolicy;
}

+ (instancetype)policyWithPinningMode:(AFSSLPinningMode)pinningMode {
    return [self policyWithPinningMode:pinningMode withPinnedCertificates:[self defaultPinnedCertificates]];
}

+ (instancetype)policyWithPinningMode:(AFSSLPinningMode)pinningMode withPinnedCertificates:(NSSet<NSData *> *)pinnedCertificates {
    AFSecurityPolicy *securityPolicy = [[self alloc] init];
    securityPolicy.SSLPinningMode = pinningMode;
    
    [securityPolicy setPinnedCertificates:pinnedCertificates];
    
    return securityPolicy;
}

// 设置证书数据
- (void)setPinnedCertificates:(NSSet<NSData *> *)pinnedCertificates {
    _pinnedCertificates = pinnedCertificates;
    
    if (self.pinnedCertificates) {
        NSMutableSet *mutablePinnedPublicKeys = [NSMutableSet setWithCapacity:[self.pinnedCertificates count]];
        for (NSData *certificate in self.pinnedCertificates) {
            // 从证书中提取出公钥
            id publickLey = AFPublicKeyForCertificate(certificate);
            if (!publickLey) {
                continue;
            }
            [mutablePinnedPublicKeys addObject:publickLey];
        }
        self.pinnedCertificates = [NSSet setWithSet:mutablePinnedPublicKeys];
    } else {
        self.pinnedPublicKeys = nil;
    }
}

// 获取默认的证书
+ (NSSet *)defaultPinnedCertificates {
    static NSSet *_defaultPinnedCertificates = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        NSBundle *bundle = [NSBundle bundleForClass:[self class]];
        _defaultPinnedCertificates = [self certificatesInBundle:bundle];
    });
    
    return _defaultPinnedCertificates;
}


@end
