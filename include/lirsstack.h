/*
 * Replacement uses the LIRS algorithm (Jiang-Zhang).
 * 
 * The "recency" of a page is the time between now and the last request
 * for the page.  The "inter-reference recency" (IRR) of a page is the
 * time between the last 2 requests for the page.  "Time" is advanced by
 * request for any page.
 *
 * Page entries are divided into "hot" ("lir") and "cold" ("hir").  Here
 * lir/hir refers to low/high IRR.  Hot pages are always resident but
 * cold pages need not be.
 *
 * Number of hot pages is limited to slightly less than number of cache
 * pages.  Until this number is reached, all used cache pages are hot.
 * Then the algorithm described next is applied.  The algorithm avoids
 * storing any of the actual recency values.
 *
 * Primary data structure is the "stack".  It contains all hot entries
 * and recently referenced cold entries (resident or not).  The stack is
 * in recency order with most recent (lowest recency) entry on top.
 * Entries which are less recent than the least recent hot page are
 * removed ("stack pruning").  So the bottom page is always hot.
 *
 * The cold entries on the stack are undergoing a "trial period".  If
 * they are referenced soon again (see IRR), they become hot.  Otherwise
 * they fall off the bottom of the stack.
 *
 * Secondary data structure is the "queue".  It contains all resident
 * cold pages (on stack or not).  When a hot page is removed from the
 * stack it is added to the end of the queue.  When page replacement
 * needs a page it removes it from the front of the queue.
 *
 * Page requests cause the input entry to be inserted and updated in
 * LIRS lists.  Remember that an entry can be present on both stack and
 * queue.  The rules per type of input entry are:
 *
 * 1. Hot.  Move input entry to stack top.  If input entry was at stack
 * bottom, do stack pruning.
 *
 * 2. Cold resident.  Move input entry to stack top.  Then:
 *
 * 2a. If input entry was on stack, change it to hot, remove it from
 * queue, change stack bottom entry to cold and move the bottom entry to
 * queue end, and do stack pruning.
 *
 * 2b. If input entry was on queue only, leave it cold but move it to
 * end of queue.
 *
 * 3. Cold non-resident.  Remove entry at queue front and evict it from
 * the cache.  If the evicted entry was on stack, it remains as unbound
 * entry on stack, to continue its trial period.  Map input entry to the
 * freed cache page.  Move input entry to stack top.  Then:
 *
 * 3a. If input entry was on stack, change it to hot, change stack
 * bottom entry to cold and move the bottom entry to queue end, and do
 * stack pruning.
 *
 * 3b. If input entry was new, leave it cold but move it to end of
 * queue.
 *
 * LIRS CHANGES
 *
 * In LIRS the 'resident' requirement is changed as follows:
 *
 * Stack entries, including hot ones, can have any state.  Unbound stack
 * entries are created by new requests and by pages evicted from queue
 * front which are still on stack.
 *
 * Queue entries must be bound.  They become resident and evictable
 * within a finite time.  A page is "evictable" if it is mapped, clean,
 * and has no requests.
 *
 * An unbound entry which should be on queue is added there at bind
 * time.  Such entries are created when an unbound entry with open
 * requests is popped (hot) or pruned (cold) from the stack.  This can
 * happen if the cache is too small.
 */
