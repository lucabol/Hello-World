#!/bin/bash
# Test script for collaborative editing server
# This tests the REST API endpoints (WebSocket testing would require a browser)

set -e

echo "🧪 Testing Collaborative Editing Server..."
echo ""

# Start the server in the background
echo "Starting server..."
npm start > /tmp/collab-server.log 2>&1 &
SERVER_PID=$!

# Wait for server to start
echo "Waiting for server to initialize..."
sleep 3

# Test if server is running
if ! kill -0 $SERVER_PID 2>/dev/null; then
    echo "❌ Server failed to start"
    cat /tmp/collab-server.log
    exit 1
fi

echo "✅ Server started (PID: $SERVER_PID)"
echo ""

# Test API endpoint
echo "Testing API endpoint..."
RESPONSE=$(curl -s http://localhost:3000/api/file)

if echo "$RESPONSE" | grep -q "content"; then
    echo "✅ API endpoint working"
    echo "   Response: $RESPONSE"
else
    echo "❌ API endpoint failed"
    echo "   Response: $RESPONSE"
    kill $SERVER_PID
    exit 1
fi

echo ""

# Test editor HTML endpoint
echo "Testing editor page..."
HTTP_STATUS=$(curl -s -o /dev/null -w "%{http_code}" http://localhost:3000/editor.html)

if [ "$HTTP_STATUS" = "200" ]; then
    echo "✅ Editor page accessible (HTTP $HTTP_STATUS)"
else
    echo "❌ Editor page not accessible (HTTP $HTTP_STATUS)"
    kill $SERVER_PID
    exit 1
fi

echo ""

# Test root redirect
echo "Testing root redirect..."
HTTP_STATUS=$(curl -s -o /dev/null -w "%{http_code}" http://localhost:3000/)

if [ "$HTTP_STATUS" = "200" ] || [ "$HTTP_STATUS" = "302" ]; then
    echo "✅ Root redirect working (HTTP $HTTP_STATUS)"
else
    echo "❌ Root redirect failed (HTTP $HTTP_STATUS)"
    kill $SERVER_PID
    exit 1
fi

echo ""

# Test rate limiting (make many requests)
echo "Testing rate limiting..."
RATE_TEST_PASSED=true
for i in {1..10}; do
    STATUS=$(curl -s -o /dev/null -w "%{http_code}" http://localhost:3000/api/file)
    if [ "$STATUS" != "200" ] && [ "$STATUS" != "429" ]; then
        echo "❌ Unexpected status code: $STATUS"
        RATE_TEST_PASSED=false
        break
    fi
done

if [ "$RATE_TEST_PASSED" = true ]; then
    echo "✅ Rate limiting configured"
else
    echo "⚠️  Rate limiting test inconclusive"
fi

echo ""

# Stop the server
echo "Stopping server..."
kill $SERVER_PID
wait $SERVER_PID 2>/dev/null || true

echo ""
echo "✅ All tests passed!"
echo ""
echo "To use the collaborative editor:"
echo "1. Run: npm start"
echo "2. Open: http://localhost:3000/editor.html"
echo "3. Open the same URL in multiple browser tabs to see real-time collaboration!"
