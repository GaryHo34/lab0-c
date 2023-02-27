#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head) * 1);
    if (head)
        INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;

    element_t *node, *safe;
    list_for_each_entry_safe (node, safe, l, list) {
        q_release_element(node);
    }

    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;
    element_t *ele = malloc(sizeof(element_t));
    if (!ele)
        return false;
    ele->value = malloc(sizeof(char) * strlen(s) + 1);
    if (!ele->value) {
        q_release_element(ele);
        return false;
    }
    strncpy(ele->value, s, strlen(s) + 1);
    list_add(&ele->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;
    element_t *ele = malloc(sizeof(element_t));
    if (!ele)
        return false;
    ele->value = malloc(sizeof(char) * strlen(s) + 1);
    if (!ele->value) {
        q_release_element(ele);
        return false;
    }
    strncpy(ele->value, s, strlen(s) + 1);
    list_add_tail(&ele->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head))
        return NULL;
    element_t *ele = list_first_entry(head, element_t, list);
    if (sp && ele->value) {
        strncpy(sp, ele->value, bufsize - 1);
    }
    list_del(&ele->list);
    return ele;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || list_empty(head))
        return NULL;
    element_t *ele = list_last_entry(head, element_t, list);
    if (sp && ele->value) {
        strncpy(sp, ele->value, bufsize - 1);
    }
    list_del(&ele->list);
    return ele;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    int c = 0;
    struct list_head *node;
    list_for_each (node, head)
        c++;
    return c;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (head == NULL || list_empty(head))
        return NULL;
    struct list_head *fast, *slow;
    fast = slow = head->next;
    while (fast->next != head && fast->next->next != head) {
        fast = fast->next->next;
        slow = slow->next;
    }
    element_t *ele = list_entry(slow, element_t, list);
    list_del(&ele->list);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (head == NULL || list_empty(head))
        return false;
    element_t *node, *safe;
    list_for_each_entry_safe (node, safe, head, list) {
        bool match =
            node->list.next != head &&
            strcmp(node->value,
                   list_entry(node->list.next, element_t, list)->value);
        if (match) {
            list_del(&node->list);
            q_release_element(node);
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (head == NULL || list_empty(head))
        return;
    struct list_head *curr;
    curr = head->next;
    while (curr != head && curr->next != head) {
        struct list_head *tmp1 = curr->next;
        struct list_head *tmp2 = tmp1->next;
        list_del_init(curr);
        curr->next = tmp2;
        curr->prev = tmp1;
        tmp1->next = curr;
        tmp2->prev = curr;
        curr = tmp2;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (head == NULL || list_empty(head))
        return;
    struct list_head *node, *safe;
    list_for_each_safe (node, safe, head)
        list_move(node, head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
