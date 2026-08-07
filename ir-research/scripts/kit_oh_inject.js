/**
 * KIT-OH Browser Injection Script
 * Inject once per browser session via: spotify_cli browser <id> evaluate "$(cat kit_oh_inject.js)"
 * Re-inject after page navigation or CDP reconnect.
 *
 * Usage from evaluate:
 *   window.__KIT__.sendMessage("text")    → sends a message to the active chat
 *   window.__KIT__.getLastMessages(n)     → returns last n messages as a string
 *   window.__KIT__.getStatus()            → returns current agent status string
 *   window.__KIT__.isIdle()               → true if input+submit are available
 *   window.__KIT__.getConversationUrl()   → returns current conversation URL
 */

window.__KIT__ = {

  sendMessage: (text) => {
    const input = document.querySelector('[data-testid="chat-input"]');
    if (!input) return 'error: chat-input not found';
    const nativeSet = Object.getOwnPropertyDescriptor(window.HTMLElement.prototype, 'innerText').set;
    nativeSet.call(input, text);
    input.dispatchEvent(new Event('input', { bubbles: true }));
    const btn = document.querySelector('[data-testid="submit-button"]');
    if (!btn) return 'error: submit-button not found';
    if (btn.disabled) return 'error: submit-button disabled (still running?)';
    btn.click();
    return 'sent';
  },

  getLastMessages: (n = 3) => {
    const msgs = [...document.querySelectorAll('[data-testid="agent-message"],[data-testid="user-message"]')];
    if (msgs.length === 0) return 'no messages';
    return msgs.slice(-n).map(e => {
      const role = e.getAttribute('data-testid') === 'user-message' ? 'USER' : 'AGENT';
      return role + ': ' + e.innerText.substring(0, 400);
    }).join('\n---\n');
  },

  getStatus: () => {
    // Check for running task indicator
    const allText = document.body.innerText;
    if (allText.includes('Running task')) return 'running';
    if (allText.includes('Waiting for task')) return 'idle';
    // Fallback: check submit button state
    const btn = document.querySelector('[data-testid="submit-button"]');
    if (btn && !btn.disabled) return 'idle';
    if (btn && btn.disabled) return 'running';
    return 'unknown';
  },

  isIdle: () => {
    const inputEl = document.querySelector('[data-testid="chat-input"]');
    const submitBtn = document.querySelector('[data-testid="submit-button"]');
    return !!(inputEl && submitBtn && !submitBtn.disabled);
  },

  getConversationUrl: () => window.location.href,

  // Check if the last agent message contains a completion marker
  isComplete: () => {
    const msgs = [...document.querySelectorAll('[data-testid="agent-message"]')];
    if (msgs.length === 0) return false;
    const last = msgs[msgs.length - 1].innerText;
    return last.includes('## Task Completed Successfully');
  },

  // Get the full last agent message (for completion output)
  getLastAgentMessage: () => {
    const msgs = [...document.querySelectorAll('[data-testid="agent-message"]')];
    if (msgs.length === 0) return 'no agent messages';
    return msgs[msgs.length - 1].innerText;
  }

};

'__KIT__ v1 injected — methods: sendMessage, getLastMessages, getStatus, isIdle, getConversationUrl, isComplete, getLastAgentMessage';
