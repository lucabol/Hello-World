#!/usr/bin/env node
/**
 * Comprehensive test suite for collaborative editor functionality
 * Orchestrates all security, OT, and functionality tests
 */

const { runOTTests } = require('./test-ot');
const { runSecurityTests } = require('./test-security');

console.log('🧪 Running Comprehensive Collaborative Editor Tests...\n');

// Main test orchestrator
async function runAllTests() {
    try {
        console.log('═══════════════════════════════════════════════════════');
        console.log('🧮 OPERATIONAL TRANSFORMATION TESTS');
        console.log('═══════════════════════════════════════════════════════');
        const otSuccess = runOTTests();
        if (!otSuccess) {
            throw new Error('Operational Transformation tests failed');
        }
        
        console.log('═══════════════════════════════════════════════════════');
        console.log('🔒 SECURITY AND SANDBOXING TESTS');  
        console.log('═══════════════════════════════════════════════════════');
        const securitySuccess = await runSecurityTests();
        if (!securitySuccess) {
            throw new Error('Security tests failed');
        }
        
        console.log('═══════════════════════════════════════════════════════');
        console.log('✅ ALL TESTS COMPLETED SUCCESSFULLY');
        console.log('═══════════════════════════════════════════════════════');
        console.log('🎉 Collaborative editor validation complete!');
        console.log('✨ Security controls and OT correctness verified');
        console.log('');
        
        return true;
    } catch (error) {
        console.error('❌ Test suite failed:', error.message);
        return false;
    }
}

// Run tests if called directly
if (require.main === module) {
    runAllTests().then(success => {
        process.exit(success ? 0 : 1);
    });
}

module.exports = { runAllTests };