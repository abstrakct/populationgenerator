set makeprg=make\ debug\ -j8
nmap <F5> :wa<CR>:cd ..<CR>:make!<CR>:cd src<CR>
imap <F5> <ESC>:wa<CR>:cd ..<CR>:make!<CR>:cd src<CR>
nmap <F6> :wa<CR>:cd ..<CR>:make!<CR>:!./dsrl<CR>:cd src<CR>
imap <F6> <ESC>:wa<CR>:cd ..<CR>:make!<CR>:!./dsrl<CR>:cd src<CR>
nmap <F7> :!../dsrl<CR>
nmap <F8> :YcmCompleter FixIt<CR>
nmap <F9> :YcmCompleter GetType<CR>

let g:ycm_confirm_extra_conf = 0
let g:ycm_min_num_of_chars_for_completion = 1
let g:ycm_key_invoke_completion = '<C-b>'
let g:ycm_server_python_interpreter = "/usr/bin/python2"
