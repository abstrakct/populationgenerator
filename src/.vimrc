set makeprg=make\ debug\ -j8
nmap <F5> :wa<CR>:cd ..<CR>:make!<CR>:cd src<CR>
imap <F5> <ESC>:wa<CR>:cd ..<CR>:make!<CR>:cd src<CR>
nmap <F6> :wa<CR>:cd ..<CR>:make!<CR>:!./dsrl<CR>:cd src<CR>
imap <F6> <ESC>:wa<CR>:cd ..<CR>:make!<CR>:!./dsrl<CR>:cd src<CR>
nmap <F7> :!../dsrl<CR>
nmap <F8> :YcmCompleter FixIt<CR>
nmap <F9> :YcmCompleter GetType<CR>

augroup project
    autocmd!
    autocmd BufRead,BufNewFile *.h,*.c   set filetype=c.doxygen
    autocmd BufRead,BufNewFile *.h,*.cpp set filetype=cpp.doxygen
augroup END

let g:ycm_confirm_extra_conf = 0
let g:ycm_min_num_of_chars_for_completion = 1
let g:ycm_key_invoke_completion = '<C-b>'
