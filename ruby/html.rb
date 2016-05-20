class Html

    DEFAULT_BROWSER = 'firefox'

    def run file , args
        if args.empty ?
            `#{DEFAULT_BROWSER} #{file}`
        else
            despatch_on_parameters file , args
        end
    end

    def dispatch_on_parameters file , args 
        cmd = args.shift
        send "do_#{cmd}" , file , args
    end

    def do_opera file , args = nil
        system "opera #{file} #{args}"
    end

    def do_konq file , args = nil
        system "konqueror #{file} #{args}"
    end
end